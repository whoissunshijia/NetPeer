// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#include "NetPeerPrivatePCH.h"
#include "TcpNetPeer.h"
#include "Sockets.h"
#include "SocketSubsystem.h"


/** Defines a magic number for the the TCP message transport. */
#define TCP_NETPEER_PROTOCOL_MAGIC 0x45504943
#define TCP_NETPEER_VERSION 0

/** Header sent over the connection as soon as it's opened */
struct FNetPeerMessageHeader
{
	uint32 MagicNumber;
	uint8 Version;
	FGuid NodeId;

	FNetPeerMessageHeader()
		: MagicNumber(0)
		, Version(0)
	{}
	
	bool IsValid() const
	{
		return
			MagicNumber == TCP_NETPEER_PROTOCOL_MAGIC &&
			Version == TCP_NETPEER_VERSION 
			// && NodeId.IsValid()
			;
	}

	FGuid GetNodeId() const
	{
		return NodeId;
	}

	uint32 GetVersion() const
	{
		return Version;
	}

	// Serializer
	friend FArchive& operator<<(FArchive& Ar, FNetPeerMessageHeader& H)
	{
		return Ar << H.MagicNumber 
				  << H.Version
				 // << H.NodeId
				 ;
	}
};

const bool ShouldHaveHeader = true;		// Should have the first packet.

UTcpNetPeer::UTcpNetPeer() : pSocket(nullptr)
	, ConnectionState(STATE_Idle)
	, OpenedTime(FDateTime::UtcNow())
	, bSentHeader(ShouldHaveHeader)
	, bReceivedHeader(ShouldHaveHeader)
	, Thread(nullptr)
	, TotalBytesReceived(0)
	, TotalBytesSent(0)	
	, bRun(false)
	, RecvMessageDataRemaining(0)
	, ConnectionRetryTimes(3)
	, ConnectingTime(5.f)
{	
}

UTcpNetPeer::~UTcpNetPeer()
{	
}

void UTcpNetPeer::BeginDestroy()
{
	Super::BeginDestroy();

	Disconnect();
}

FString UTcpNetPeer::GetDesc()
{
	return FString::Printf(TEXT("Tcp to %s"), *m_strServerIpAndPort);
}

void UTcpNetPeer::PostLoad()
{
	Super::PostLoad();

	// Bind the default private message handler.
	for (uint16 msgId = (uint16)ENetMessageId::ConnectSuccessed; msgId != (uint16)ENetMessageId::ReconnectFailed; msgId++)
	{
		RebindPrivateMessage(msgId, nullptr);
	}
}

void UTcpNetPeer::Connect_BP(UObject* WorldContextObject, FLatentActionInfo LatentInfo, EConnectResult& Succeed, const FString& ServerIp, int32 Port, float _ConnectingTime /*= 5.f*/)
{
	FString strServerIpAndPort = FString::Printf(TEXT("%s:%d"), *ServerIp, Port);
	
	ConnectEx_BP(WorldContextObject, LatentInfo, Succeed, strServerIpAndPort, _ConnectingTime);
}

void UTcpNetPeer::ConnectEx_BP(UObject* WorldContextObject, FLatentActionInfo LatentInfo, EConnectResult& ConnectResult, const FString& ServerIpAndPort /*= TEXT("127.0.0.1:2016")*/, float _ConnectingTime /*= 5.f*/)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject);
	if (world == nullptr)
	{
		UE_LOG(LogNetPeer, Error, TEXT("Can't find the world when connect to server (IpAndPort=%s)"), *ServerIpAndPort);
		return;
	}

	FName strExecutionFunction = LatentInfo.ExecutionFunction;
	int32 LinkID = LatentInfo.Linkage;
	FWeakObjectPtr CallbackTargetWeakPtr = LatentInfo.CallbackTarget;

	if (LinkID != INDEX_NONE && strExecutionFunction != NAME_None && NULL != LatentInfo.CallbackTarget)
	{
		BindMessageHandler_0((FMessageIdType)ENetMessageId::ConnectSuccessed, [this, CallbackTargetWeakPtr, strExecutionFunction, LinkID, &ConnectResult]() {
			ConnectResult = EConnectResult::OnSuccess;
			ConnectionState = EConnectionState::STATE_Connected;
			return ResumeBlueprintExec(CallbackTargetWeakPtr, strExecutionFunction, LinkID);
		}
		);

		BindMessageHandler_0((FMessageIdType)ENetMessageId::ConnectFailed, [this, CallbackTargetWeakPtr, strExecutionFunction, LinkID, &ConnectResult]() {
			ConnectResult = EConnectResult::OnFailure;
			ConnectionState = EConnectionState::STATE_ConnectFailed;
			return ResumeBlueprintExec(CallbackTargetWeakPtr, strExecutionFunction, LinkID);
		}
		);
	}

	ConnectingTime = _ConnectingTime;
	Connect(ServerIpAndPort);
}

void UTcpNetPeer::BindSysMessage(UObject* WorldContextObject, FLatentActionInfo LatentInfo, ENetMessageId MessageId)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject);
	if (world == nullptr)
	{
		UE_LOG(LogNetPeer, Error, TEXT("Can't find the world when bind system message(%s)"), *GetDesc());
		return;
	}
	
	FName strExecutionFunction = LatentInfo.ExecutionFunction;
	int32 LinkID = LatentInfo.Linkage;
	FWeakObjectPtr CallbackTargetWeakPtr = LatentInfo.CallbackTarget;

	BindMessageHandler_0((FMessageIdType)MessageId, [=]() {		
		return ResumeBlueprintExec(CallbackTargetWeakPtr, strExecutionFunction, LinkID);
	}
	);
}

bool UTcpNetPeer::Connect(const FString& ServerIpAndPort)
{
	Disconnect();

	TickHandler = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UTcpNetPeer::Tick));
	
	if (!ReConnect(ServerIpAndPort))
	{
		// Push a connect failed message.
		Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::ConnectFailed)));
		return false;
	}

	// Push a connecting message.
	Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::Connecting)));
	ConnectionState = EConnectionState::STATE_Connecting;
	
	// Start a thread for socket.
	check(Thread == nullptr);
	bRun = true;
	Thread = FRunnableThread::Create(this, TEXT("NetPeer"), 128 * 1024, TPri_Normal);
	return true;
}

bool UTcpNetPeer::ReConnect(const FString& ServerIpAndPort)
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (SocketSubsystem == nullptr)
	{
		check(false);
		return false;
	}

	if (pSocket != nullptr)
	{
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(pSocket);
		pSocket = nullptr;
	}

	FString Description = FString::Printf(TEXT("Tcp to connect to %s"), *ServerIpAndPort);
	pSocket = SocketSubsystem->CreateSocket(NAME_Stream, *Description, false);
	if (pSocket == nullptr)
	{
		UE_LOG(LogNetPeer, Error, TEXT("Create socket failed(%s)"), *ServerIpAndPort);
		return false;
	}

	do
	{
		bool bError = !pSocket->SetReuseAddr(false) ||
			!pSocket->SetLinger(false, 0) ||
			!pSocket->SetRecvErr() ||
			!pSocket->SetNonBlocking(true);

		if (bError)
		{
			UE_LOG(LogNetPeer, Error, TEXT("Failed to set socket attrib. (%s)"), *ServerIpAndPort);
			break;
		}

		auto inAddr = SocketSubsystem->CreateInternetAddr();
		bool bValid = false;;
		inAddr->SetIp(*ServerIpAndPort, bValid);
		if (!bValid)
		{
			UE_LOG(LogNetPeer, Error, TEXT("Failed to parase server ip and port. (%s)"), *ServerIpAndPort);
			break;
		}

		m_strServerIpAndPort = ServerIpAndPort;
		
		pSocket->Connect(inAddr.Get());
			
		return true;

	} while (0);

	SocketSubsystem->DestroySocket(pSocket);
	pSocket = nullptr;
	return false;
}

FString UTcpNetPeer::GetName() const
{
	return m_strServerIpAndPort;
}

void UTcpNetPeer::BindMessageHandler(FMessageIdType MessageId, const FMessageHeadlerFunction& pfnCallback)
{
	if (MessageHandles.Find(MessageId) != nullptr && MessageId >= static_cast<FMessageIdType>(ENetMessageId::UserMessageIdStart))
	{
		UE_LOG(LogNetPeer, Warning, TEXT("The handle function of message(id=%d) is already exist, it will be replaced."), MessageId);
	}
	MessageHandles.Emplace(MessageId, pfnCallback);
}

void UTcpNetPeer::BindMessageHandler_0(FMessageIdType MessageId, const TFunction<void ()>& pfnCallback)
{
	FMessageHeadlerFunction pfnRebind = RebindPrivateMessage(MessageId, pfnCallback);
	if (pfnRebind)
	{
		BindMessageHandler(MessageId, pfnRebind);
		return;
	}

	if (MessageHandles.Contains(MessageId))
	{
		UE_LOG(LogNetPeer, Warning, TEXT("The handle function of message(id=%d) is already exist, it will be replaced."), MessageId);
	}
	
	MessageHandles.Emplace(MessageId, [=](const FMessagePacketPtr&)->void {
		pfnCallback();
	}
	);
}	

FMessageHeadlerFunction UTcpNetPeer::RebindPrivateMessage(FMessageIdType MessageId, const TFunction<void()>& pfnCallback)
{
	switch ((ENetMessageId)MessageId)
	{
	case ENetMessageId::Connecting:
		return [this, pfnCallback](const FMessagePacketPtr&) {
			ConnectionState = EConnectionState::STATE_Connecting;
			if(pfnCallback)
				pfnCallback();
		};
		break;

	case ENetMessageId::ConnectLost:
		return [this, pfnCallback](const FMessagePacketPtr&) {
			ConnectionState = EConnectionState::STATE_Disconnected;
			if (pfnCallback)
				pfnCallback();
		};
		break;

	case ENetMessageId::Reconnecting:
		return [this, pfnCallback](const FMessagePacketPtr&) {
			ConnectionState = EConnectionState::STATE_DisconnectReconnectPending;
			if (pfnCallback)
				pfnCallback();
		};
		break;
	case ENetMessageId::ReconnectSuccessed:
		return [this, pfnCallback](const FMessagePacketPtr&) {
			ConnectionState = EConnectionState::STATE_Connected;
			if (pfnCallback)
				pfnCallback();
		};
		break;
	case ENetMessageId::ReconnectFailed:
		return [this, pfnCallback](const FMessagePacketPtr&) {
			ConnectionState = EConnectionState::STATE_Disconnected;
			if (pfnCallback)
				pfnCallback();
		};
		break;
	default:
		return nullptr;
		break;
	}	
}

bool UTcpNetPeer::ResumeBlueprintExec(const FWeakObjectPtr& CallbackTargetWeakPtr, const FName& strExecutionFunction, int32 LinkID)
{
	if (UObject* CallbackTarget = CallbackTargetWeakPtr.Get())
	{
		if (UFunction* ExecutionFunction = CallbackTarget->FindFunction(strExecutionFunction))
		{
			const bool bInstrumentLatentCall = CallbackTarget->GetClass()->HasInstrumentation();
			if (bInstrumentLatentCall)
			{
				// Place event markers around the latent blueprint call so we can match up the entry points.
				FScriptInstrumentationSignal LatentEventStartInfo(EScriptInstrumentation::ResumeEvent, CallbackTarget, ExecutionFunction, LinkID);
				FBlueprintCoreDelegates::InstrumentScriptEvent(LatentEventStartInfo);
				CallbackTarget->ProcessEvent(ExecutionFunction, (void*)&LinkID);
				FScriptInstrumentationSignal LatentEventStopInfo(EScriptInstrumentation::Stop, CallbackTarget, ExecutionFunction);
				FBlueprintCoreDelegates::InstrumentScriptEvent(LatentEventStopInfo);
			}
			else
			{
				CallbackTarget->ProcessEvent(ExecutionFunction, (void*)&LinkID);
			}
		}
		else
		{
			UE_LOG(LogNetPeer, Warning, TEXT("UTcpNetPeer::ResumeBlueprint: Could not find latent action resume point named '%s' on '%s'"),
				*strExecutionFunction.ToString(), *(CallbackTarget->GetPathName()));
			return false;
		}
	}
	else
	{
		UE_LOG(LogNetPeer, Warning, TEXT("UTcpNetPeer::ResumeBlueprint: CallbackTarget is None."));
		return false;
	}
	return true;
}

void UTcpNetPeer::SendNullMessage(int32 MessageId)
{
	FMessageWriterPtr messageWriter = MakeShareable(new FMessagePacket(MessageId));
	Outbox.Enqueue(messageWriter);
}

void UTcpNetPeer::Disconnect()
{
	if (TickHandler.IsValid())
	{
		FTicker::GetCoreTicker().RemoveTicker(TickHandler);
		TickHandler.Reset();
	}

	if (Thread != nullptr)
	{
		if (bRun)
		{
			bRun = false;
			Thread->WaitForCompletion();
		}
		delete Thread;
		Thread = nullptr;
	}

	if (pSocket)
	{
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(pSocket);
		pSocket = nullptr;
	}

	Inbox.Empty();
	Outbox.Empty();

	ConnectionState = STATE_Idle;
	RecvMessageDataRemaining = 0;
	TotalBytesReceived = 0;
	TotalBytesSent = 0;
}


/* FRunnable interface
*****************************************************************************/

bool UTcpNetPeer::Init()
{
	return true;
}

uint32 UTcpNetPeer::Run()
{
	// Check connecting state.
	if (!CheckConnecting())
		return -1;

	while (bRun)
	{
		// Try sending and receiving messages and detect if they fail or if another connection error is reported.
		if ((!ReceiveMessages() || !SendMessages() || pSocket->GetConnectionState() == SCS_ConnectionError) && bRun)
		{			
			if (!TryReconnect())
			{
				bRun = false;
				break;
			}
		}

		FPlatformProcess::Sleep(0.0f);
	}

	// Send all the message in list.
	while (!Outbox.IsEmpty() && SendMessages() && pSocket->GetConnectionState() != SCS_ConnectionError);

	ClosedTime = FDateTime::UtcNow();
	return 0;
}

void UTcpNetPeer::Stop()
{
	if (pSocket)
	{
		pSocket->Close();
	}
}

void UTcpNetPeer::Exit()
{
	// do nothing
}


bool UTcpNetPeer::ReceiveMessages()
{
	uint32 PendingDataSize = 0;

	// check if the socket has closed
	{
		int32 BytesRead;
		uint8 Dummy;
		if (!pSocket->Recv(&Dummy, 1, BytesRead, ESocketReceiveFlags::Peek))
		{
			return false;
		}
	}

	if (!bReceivedHeader)
	{
		if (pSocket->HasPendingData(PendingDataSize) && PendingDataSize >= sizeof(FNetPeerMessageHeader))
		{
			FArrayReader HeaderData = FArrayReader(true);
			HeaderData.SetNumUninitialized(sizeof(FNetPeerMessageHeader));
			int32 BytesRead = 0;
			if (!pSocket->Recv(HeaderData.GetData(), sizeof(FNetPeerMessageHeader), BytesRead))
			{
				return false;
			}

			check(BytesRead == sizeof(FNetPeerMessageHeader));
			TotalBytesReceived += BytesRead;

			FNetPeerMessageHeader MessageHeader;
			HeaderData << MessageHeader;

			if (!MessageHeader.IsValid())
			{
				return false;
			}
			else
			{				
				bReceivedHeader = true;
				OpenedTime = FDateTime::UtcNow();
			}
		}
		else
		{
			// no header yet
			return true;
		}
	}
	
	// keep going until we have no data.
	for (;;)
	{
		int32 BytesRead = 0;
		// See if we're in the process of receiving a (large) message
		if (RecvMessageDataRemaining == 0)
		{
			// no partial message. Try to receive the size of a message
			if (!pSocket->HasPendingData(PendingDataSize) || (PendingDataSize < sizeof(FMessageLenthType)))
			{
				// no messages
				return true;
			}

			FArrayReader MessagesizeData = FArrayReader(true);
			MessagesizeData.SetNumUninitialized(sizeof(FMessageLenthType));

			// read message size from the stream
			BytesRead = 0;
			if (!pSocket->Recv(MessagesizeData.GetData(), sizeof(FMessageLenthType), BytesRead))
			{
				return false;
			}

			check(BytesRead == sizeof(FMessageLenthType));
			TotalBytesReceived += BytesRead;

			// Setup variables to receive the message
			MessagesizeData << RecvMessageDataRemaining;

			check(RecvMessageDataRemaining >= sizeof(FMessageLenthType));
			RecvMessageDataRemaining -= sizeof(FMessageLenthType);
			
			RecvMessageData = MakeShareable(new FMessagePacket());
			RecvMessageData->SetNumUninitialized(RecvMessageDataRemaining);
			check(RecvMessageDataRemaining > 0);
		}

		BytesRead = 0;
		if (!pSocket->Recv(RecvMessageData->GetRawData() + RecvMessageData->GetRawNum() - RecvMessageDataRemaining, RecvMessageDataRemaining, BytesRead))
		{
			return false;
		}

		if (BytesRead > 0)
		{
			check(BytesRead <= RecvMessageDataRemaining);
			TotalBytesReceived += BytesRead;
			RecvMessageDataRemaining -= BytesRead;
			if (RecvMessageDataRemaining == 0)
			{
				Inbox.Enqueue(RecvMessageData);
				RecvMessageData.Reset();
			}
		}
		else
		{
			// no data
			return true;
		}
	}
}

void UTcpNetPeer::UpdateConnectionState(EConnectionState NewConnectionState)
{
	ConnectionState = NewConnectionState;
	// ConnectionStateChangedDelegate.ExecuteIfBound();
}

bool UTcpNetPeer::BlockingSend(const uint8* Data, int32 BytesToSend)
{
	int32 TotalBytes = BytesToSend;
	while (BytesToSend > 0)
	{
		while (!pSocket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan(0, 0, 1)))
		{
			if (pSocket->GetConnectionState() == SCS_ConnectionError)
			{
				return false;
			}
		}

		int32 BytesSent = 0;
		if (!pSocket->Send(Data, BytesToSend, BytesSent))
		{
			return false;
		}
		BytesToSend -= BytesSent;
		Data += BytesSent;
	}
	return true;
}

bool UTcpNetPeer::CheckConnecting()
{
	if (EConnectionState::STATE_Connecting == ConnectionState)
	{
		float fWaitTime = 0.f;
		const float fWaitTimeSingle = 0.3f;
		while (bRun)
		{
			if (pSocket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan::FromSeconds(fWaitTimeSingle))
				&& ReceiveMessages() && SendMessages())
			{	// Connect successed.
				// Push a connect successed message.
				Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::ConnectSuccessed)));
				return true;
			}

			fWaitTime += fWaitTimeSingle;
			if (fWaitTime > ConnectingTime || pSocket->GetConnectionState() == SCS_ConnectionError)
			{	// Time out or connect failed.
				// Push a connect failed message.
				Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::ConnectFailed)));
				return false;
			}
		}
	}
	return false;
}

bool UTcpNetPeer::TryReconnect()
{
	// Disconnected. Reconnect if requested.
	if (ConnectionRetryTimes > 0)
	{		
		for (int32 nRetryCounter = 0; bRun && nRetryCounter < ConnectionRetryTimes; nRetryCounter++)
		{	// Try reconnect.
			UE_LOG(LogNetPeer, Verbose, TEXT("Connection to '%s' lost, reconnecting... %d"), *m_strServerIpAndPort, nRetryCounter);

			if (ReConnect(m_strServerIpAndPort))
			{
				bSentHeader = ShouldHaveHeader;
				bReceivedHeader = ShouldHaveHeader;

				if (nRetryCounter == 0)
				{	// Push a reconnecting message.
					Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::Reconnecting)));
				}

				// Wait for reconnect.
				float fWaitTime = 0.f;
				const float fWaitTimeSingle = 0.3f;
				while (bRun)
				{
					if (pSocket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan::FromSeconds(fWaitTimeSingle))
						&& ReceiveMessages() && SendMessages())
					{						
						Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::ReconnectSuccessed)));
						return true;
					}

					fWaitTime += fWaitTimeSingle;
					if (fWaitTime > ConnectingTime || pSocket->GetConnectionState() == SCS_ConnectionError)
					{
						break;
					}
				}
			}
			else
			{
				// Falt error.
				break;
			}
		}
				
		// Push a reconnect failed message.
		Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::ReconnectFailed)));		
	}
	else
	{	// Connect lost.
		Inbox.Enqueue(MakeShareable(new FMessagePacket(ENetMessageId::ConnectLost)));		
	}
	return false;
}

bool UTcpNetPeer::SendMessages()
{
	if (Outbox.IsEmpty() && bSentHeader)
	{
		return true;
	}

	if (!pSocket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan::Zero()))
	{
		return true;
	}

	if (!bSentHeader)
	{
		FArrayWriter HeaderData;
		FNetPeerMessageHeader MessageHeader;
		HeaderData << MessageHeader;

		if (!BlockingSend(HeaderData.GetData(), sizeof(FNetPeerMessageHeader)))
		{
			return false;
		}

		bSentHeader = true;
		TotalBytesSent += sizeof(FNetPeerMessageHeader);
	}
	else
	{
		FMessageWriterPtr Message;
		while (Outbox.Dequeue(Message))
		{
			int32 BytesSent = 0;
			const FMessagePacket& Payload = *Message.Get();

			// send the payload size
			FArrayWriter MessagesizeData = FArrayWriter(true);
			FMessageLenthType Messagesize = (FMessageLenthType)Payload.GetRawNum();
#ifdef USE_MPACK
			Messagesize += sizeof(FMessageLenthType);
#endif
			MessagesizeData << Messagesize;

			if (!BlockingSend(MessagesizeData.GetData(), sizeof(FMessageLenthType)))
			{
				return false;
			}

			TotalBytesSent += sizeof(FMessageLenthType);

			// send the payload
			if (!BlockingSend(Payload.GetRawData(), Payload.GetRawNum()))
			{
				return false;
			}

			TotalBytesSent += Payload.GetRawNum();

			// return if the socket is no longer writable, or that was the last message
			if (Outbox.IsEmpty() || !pSocket->Wait(ESocketWaitConditions::WaitForWrite, FTimespan::Zero()))
			{
				return true;
			}
		}
	}
	return true;
}

bool UTcpNetPeer::Tick(float DeltaTime)
{
	// Hanle the message.
	FMessagePacketPtr Message;
	while (Inbox.Dequeue(Message))
	{
		const FMessageHeadlerFunction* msgHandler = MessageHandles.Find( Message->GetMessageId() );
		if(msgHandler != nullptr)
			(*msgHandler)(Message);
		else
		{
			UE_LOG(LogNetPeer, Error, TEXT("Message (Id = %d) is not handled!"), Message->GetMessageId());
		}
	}
	return true;
}