// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Networking.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "StructNetSerializer.h"
#include "TcpNetPeer.generated.h"

// #define USE_MPACK		// Use complex message header.


// forward declarations
class FRunnableThread;

/*
 * Private message id. The id of user message must be in range (UserMessageIdStart, UserMessageIdEnd).
 */
UENUM()
enum class ENetMessageId : uint8 {	
	Invalid			= 0,	
	ConnectSuccessed,
	ConnectFailed,
	Connecting,
	ConnectLost,
	Reconnecting,
	ReconnectSuccessed,
	ReconnectFailed,

	// The base id of user message.
	UserMessageIdStart  = 100,
};

const uint16 UserMessageIdEnd = 0xFFFF - 1;				// The end id of user message.(65534)

// The data type of message length is uint16.
typedef uint16 FMessageLenthType;
const FMessageLenthType MaxMessageLength = MAX_uint16;	// The max length of a message.(Byte)
typedef uint16 FMessageIdType;							// The type of message id.

#ifdef USE_MPACK

const uint32 MAX_LPACK_SIZE = 512;

// The header of low packet.
struct LPackHeader
{
	// The count of middle packet.
	uint16  m_wPackCount;

	// Return the data of low packet.
	uint8* GetLPackData()
	{
		return (uint8*)(this + 1);
	}

	const uint8* GetLPackData() const
	{
		return (uint8*)(this + 1);
	}
};

// The size of low packet's header.
const uint32 LPACK_HEADER_SIZE = sizeof(LPackHeader);

// The size of data in low packet.
const uint32 MAX_LPACK_DATA_SIZE = MAX_LPACK_SIZE - sizeof(LPackHeader);

struct  MPackInfo
{
	// The size of middle packet.
	uint16 m_wMPackSize;

	// The flag of middle packet.
	uint16 m_wMPackFlag;
	MPackInfo()
	{
	}
};

// The header of middle packet.
struct MPackHeader
{
	enum { MAX_SIZE = MAX_LPACK_DATA_SIZE };
	
	MPackInfo	m_wMPackInfo;

	// The type of user message.
	FMessageIdType	m_wType;

	// The id of connection.
	uint16	m_wConnectionID;
	
	union
	{
		struct
		{
			uint16 m_wUserEncode : 1;
			uint16 m_wUserIdCard : 1;
			uint16 m_wEncodeType : 2;
			uint16 m_wPackID : 12;
		};
		uint16   m_wTransClientConnectID;
	};

	MPackHeader() :m_wType(-1), m_wConnectionID(-1), m_wTransClientConnectID(-1)
	{

	}


	FMessageIdType  GetMsgType() const
	{
		return m_wType;
	}

	uint8* GetMPackData() const
	{
		return (uint8*)(this + 1);
	}
};

const uint32 MPACK_HEADER_SIZE = sizeof(MPackHeader);
const uint32 PacketHanderSize = LPACK_HEADER_SIZE + MPACK_HEADER_SIZE;
#else

struct LPackHeader
{	
	FMessageIdType	MessageId;

	uint8* GetLPackData()
	{
		return (uint8*)(this + 1);
	}

	const uint8* GetLPackData() const
	{
		return (uint8*)(this + 1);
	}
};

const uint32 LPACK_HEADER_SIZE = sizeof(LPackHeader);
const uint32 MPACK_HEADER_SIZE = 0;
const uint32 PacketHanderSize = LPACK_HEADER_SIZE + MPACK_HEADER_SIZE;

#endif

/*
 * The packet of message.
 */
struct FMessagePacket
{	
	TArray<uint8> MessageBody;		// The body of packet. Message id is stored in the first two bytes.

	FMessagePacket()
	{
	}

	FMessagePacket(ENetMessageId MsgId)
	{
		Init((FMessageIdType)MsgId);
	}

	FMessagePacket(uint16 MsgId)
	{
		Init((FMessageIdType)MsgId);
	}

	void Init(FMessageIdType MsgId)
	{
		MessageBody.AddUninitialized(PacketHanderSize);
#ifdef USE_MPACK
		GetLPackHeader()->m_wPackCount = 1;
		MPackHeader* pMHeader = (MPackHeader*)(GetLPackHeader()->GetLPackData());
		FMemory::Memzero(*pMHeader);
		pMHeader->m_wType = MsgId;
#else
		(*GetLPackHeader()).MessageId = (FMessageIdType)MsgId;
#endif
	}

	void SetNumUninitialized(int32 NewNum)
	{
		MessageBody.SetNumUninitialized(NewNum);
	}

	LPackHeader* GetLPackHeader()
	{
		return (LPackHeader*)MessageBody.GetData();
	}

	const LPackHeader* GetLPackHeader() const
	{
		return (LPackHeader*)MessageBody.GetData();
	}

	// Return the id of message.
	FMessageIdType GetMessageId() const
	{
#ifdef USE_MPACK
		const MPackHeader* pMHeader = (MPackHeader*)(GetLPackHeader()->GetLPackData());
		return pMHeader->GetMsgType();
#else
		return (*(LPackHeader*)MessageBody.GetData()).MessageId;
#endif
	}

	// Return the body of message(without message id).
	const uint8* GetData() const
	{
		return MessageBody.GetData() + PacketHanderSize;
	}
	
	// Return the length of message(without message id).
	int32 Num() const
	{
		return MessageBody.Num() - PacketHanderSize;
	}

	// Return the body of message(with message id).
	uint8* GetRawData()
	{
		return MessageBody.GetData();
	}

	// Return the body of message(with message id).
	const uint8* GetRawData() const
	{
		return MessageBody.GetData();
	}
	
	// Return the length of message(with message id).
	int32 GetRawNum() const
	{
		return MessageBody.Num();
	}

	// Write the data.
	void write(const char* buf, size_t len)
	{
		int32 nOffset = MessageBody.Num();
		MessageBody.AddUninitialized(len);
		FMemory::Memcpy(MessageBody.GetData() + nOffset, buf, len);
	}
	
	void FinishWrite()
	{
		check(MessageBody.Num() >= PacketHanderSize);

#ifdef USE_MPACK
		LPackHeader* pLHeader = GetLPackHeader();		
		MPackHeader* pMHeader = (MPackHeader*)(pLHeader->GetLPackData());
		pMHeader->m_wMPackInfo.m_wMPackSize = (uint16)(MessageBody.Num() - LPACK_HEADER_SIZE);
#endif
	}
};

typedef TSharedPtr< FMessagePacket > FMessagePacketPtr;
typedef TSharedPtr< FMessagePacket > FMessageWriterPtr;

typedef TFunction<void(const FMessagePacketPtr&)> FMessageHeadlerFunction;


/*
 *	Dumy struct for blueprint.
 */
USTRUCT(Blueprintable, BlueprintType)
struct FNetMessageDumyStruct
{
	GENERATED_USTRUCT_BODY()
public:
	FNetMessageDumyStruct() { }
};

/** Enum used to indicate success or failure of EvaluateCurveTableRow. */
UENUM()
enum class EConnectResult : uint8 {	
	OnSuccess,
	OnFailure,
};

/**
* A tcp client connection.
*/
UCLASS(Blueprintable, BlueprintType)
class NETPEER_API UTcpNetPeer : public UObject, public FRunnable
{
	GENERATED_BODY()
protected:
	UTcpNetPeer();
	virtual ~UTcpNetPeer();

	//~ UObject interface

	virtual void BeginDestroy() override;
	virtual FString GetDesc() override;
	virtual void PostLoad() override;

public:

	/** Connection state */
	enum EConnectionState
	{
		STATE_Idle,							// idling.
		STATE_Connecting,					// connecting but don't yet have RemoteNodeId.
		STATE_Connected,					// connected and RemoteNodeId is valid.
		STATE_ConnectFailed,				// connect failed.
		STATE_DisconnectReconnectPending,	// disconnected with reconnect pending. Previous RemoteNodeId is retained.
		STATE_Disconnected					// disconnected. Previous RemoteNodeId is retained.
	};

	 /**
	 * Connect to a tcp server.
	 *
	 * @param	WorldContextObject	Context object.
	 * @param	LatentInfo			The info of latent.
	 * @param	ServerIp			The IP of tcp server to connect, like 192.168.1.88.
	 * @param	Port				The port of tcp server to connect, like 1008.
	 *								IPv6 - [1111:2222:3333:4444:5555:6666:7777:8888]:PORT || [1111:2222:3333::]:PORT || [::ffff:IPv4]:PORT
	 *								IPv4 - aaa.bbb.ccc.ddd:PORT || 127.0.0.1:1234:PORT
	 * @param	Succeed				true for connect success, false for connect failed.
	 * @param	ConnectingTime		The max time to connect(second)	 
	 **/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp", meta = (WorldContext = "WorldContextObject", Latent, LatentInfo = "LatentInfo", DisplayName="Connect", ExpandEnumAsExecs = "ConnectResult"))
	void Connect_BP(UObject* WorldContextObject, FLatentActionInfo LatentInfo, EConnectResult& ConnectResult, const FString& ServerIp = TEXT("127.0.0.1"), int32 Port=2016, float ConnectingTime = 5.f);

	/**
	* Connect to a tcp server.
	*
	* @param	WorldContextObject	Context object.
	* @param	LatentInfo			The info of latent.
	* @param	ServerIpAndPort		The IP of tcp server to connect, like 192.168.1.88:2016.
	*								IPv6 - [1111:2222:3333:4444:5555:6666:7777:8888]:PORT || [1111:2222:3333::]:PORT || [::ffff:IPv4]:PORT
	*								IPv4 - aaa.bbb.ccc.ddd:PORT || 127.0.0.1:1234:PORT
	* @param	Succeed				true for connect success, false for connect failed.
	* @param	ConnectingTime		The max time to connect(second)
	**/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp", meta = (WorldContext = "WorldContextObject", Latent, LatentInfo = "LatentInfo", DisplayName = "ConnectEx", ExpandEnumAsExecs = "ConnectResult"))
	void ConnectEx_BP(UObject* WorldContextObject, FLatentActionInfo LatentInfo, EConnectResult& ConnectResult, const FString& ServerIpAndPort = TEXT("127.0.0.1:2016"), float ConnectingTime = 5.f);

	 /**
	 * Recive a user defined message.
	 *
	 * @param	WorldContextObject	Context object
	 * @param	LatentInfo			The info of latent.
	 * @param	MessageId			The id of the message to recive. 100 < id < 65535.
	 * @param	MessagePacket		The message's data struct to recive.
	 **/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp", CustomThunk, meta = (WorldContext = "WorldContextObject", Latent, LatentInfo = "LatentInfo", CustomStructureParam = "MessagePacket"))
	void BindNetMessage(UObject* WorldContextObject, FLatentActionInfo LatentInfo, int32 MessageId, FNetMessageDumyStruct& MessagePacket);

	DECLARE_FUNCTION(execBindNetMessage)
	{
		P_GET_OBJECT(UObject, WorldContextObject);
		P_GET_STRUCT(FLatentActionInfo, LatentInfo);
		P_GET_PROPERTY(UIntProperty, MessageId);

		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;
	
		Stack.StepCompiledIn<UStructProperty>(NULL);
		auto DstStructProperty = Cast<UStructProperty>(Stack.MostRecentProperty);
		void* DstStructAddr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		P_NATIVE_BEGIN;

		if (DstStructAddr == nullptr || DstStructProperty == nullptr)
		{
			FFrame::KismetExecutionMessage(
				*FString::Printf(
					TEXT("Not find the type of message packet(MessageId=%d). Please connect a 'Break %s' node.\n%s"),
					MessageId,
					Stack.CurrentNativeFunction != nullptr ? *Stack.CurrentNativeFunction->GetName() : TEXT(""),
					*Stack.Object->GetFullName()
				),
				ELogVerbosity::Error
			);
			return;
		}

		if (MessageId < (int32)ENetMessageId::UserMessageIdStart || MessageId >= UserMessageIdEnd)
		{
			FFrame::KismetExecutionMessage(
				*FString::Printf(
					TEXT("Invalid message id(%d), Must be in [100 - 65535].\n%s\n%s"),
					MessageId,
					Stack.CurrentNativeFunction != nullptr ? *Stack.CurrentNativeFunction->GetName() : TEXT(""),
					*Stack.Object->GetFullName()
				),
				ELogVerbosity::Error
			);
			return;
		}

		DstStructProperty->Struct->InitializeStruct(DstStructAddr);

		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject))
		{
			FName strExecutionFunction = LatentInfo.ExecutionFunction;
			int32 LinkID = LatentInfo.Linkage;
			FWeakObjectPtr CallbackTargetWeakPtr = LatentInfo.CallbackTarget;

			if (LinkID == INDEX_NONE || strExecutionFunction == NAME_None || NULL == LatentInfo.CallbackTarget)
			{
				FFrame::KismetExecutionMessage(
					*FString::Printf(
						TEXT("Failed to bind message (MessageId=%d, LinkID=%d, strExecutionFunction=%s).\n%s\n%s"),
						MessageId,
						LinkID,
						*strExecutionFunction.ToString(),
						Stack.CurrentNativeFunction != nullptr ? *Stack.CurrentNativeFunction->GetName() : TEXT(""),
						*Stack.Object->GetFullName()
					),
					ELogVerbosity::Error
				);
				return;
			}

			FString strErrorInfo = FString::Printf(TEXT("%s:%s(%s)"), *DstStructProperty->GetName(), *DstStructProperty->Struct->GetName(), *Stack.Object->GetFullName());

			BindMessageHandler(MessageId, [=](const FMessagePacketPtr& Message) {
				// Deserialize the message.
				if (!FStructNetSerializer::MDeserialize(DstStructAddr, *DstStructProperty->Struct, Message->GetData(), Message->Num()))
				{
					UE_LOG(LogNetPeer, Error, TEXT("Deserialize message (id = %d) failed!\n%s"), MessageId, *strErrorInfo);
					return;
				}
				ResumeBlueprintExec(CallbackTargetWeakPtr, strExecutionFunction, LinkID);
			}
			);

		}
		P_NATIVE_END
	}
	
	 /**
	 * Recive a system message. like 'Reconnecting', 'Reconnect failed'.
	 *
	 * @param	WorldContextObject	Context object
	 * @param	LatentInfo			The info of latent.
	 * @param	MessageId			The id of system message to recive.
	 **/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp", meta = (WorldContext = "WorldContextObject", Latent, LatentInfo = "LatentInfo"))
	void BindSysMessage(UObject* WorldContextObject, FLatentActionInfo LatentInfo, ENetMessageId MessageId);

	 /**
	 * Send a null message without data, only the message id. simple use.
	 *
	 * @param MessageId The id of message to be send. 100 < Messageid < 65535
	 **/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp")
	void SendNullMessage(int32 MessageId);

	 /**
	 * Send a message to server.
	 *
	 * @param MessageId The id of message to be send. 100 < MessageId < 65535
	 * @param MessagePacket The data struct of message to be send. Created by "Make ****". like "Make CTOSLogin".
	 * @return true for successed, false for failed.
	 **/
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "NetPeer|Tcp", meta = (CustomStructureParam = "MessagePacket"))
	bool SendMessage(int32 MessageId, const FNetMessageDumyStruct& MessagePacket);

	DECLARE_FUNCTION(execSendMessage)
	{
		P_GET_PROPERTY(UIntProperty, MessageId);

		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;

		Stack.StepCompiledIn<UStructProperty>(NULL);
		auto MsgStructProperty = Cast<UStructProperty>(Stack.MostRecentProperty);
		void* MsgStructAddr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		P_NATIVE_BEGIN;

		if (MsgStructAddr == nullptr || MsgStructProperty == nullptr)
		{
			FFrame::KismetExecutionMessage(
				*FString::Printf(
					TEXT("The message is not find(MessageId=%d). Please connect a message's struct instance to 'MessagePacket'.\n%s"),
					MessageId,					
					*Stack.Object->GetFullName()
				),
				ELogVerbosity::Error
			);

			*(bool*)Z_Param__Result = false;
			return;
		}

		if (MessageId > UserMessageIdEnd || MessageId < (int32)ENetMessageId::UserMessageIdStart)
		{
			FFrame::KismetExecutionMessage(
				*FString::Printf(
					TEXT("Invalid message id(%d),must be in (%d - %d).\n%s\n%s"),
					MessageId,
					(int32)ENetMessageId::UserMessageIdStart,
					UserMessageIdEnd,
					Stack.CurrentNativeFunction != nullptr ? *Stack.CurrentNativeFunction->GetName() : TEXT(""),
					*Stack.Object->GetFullName()
				),
				ELogVerbosity::Error
			);
			*(bool*)Z_Param__Result = false;
			return;
		}
		
		FMessageWriterPtr messageWriter = MakeShareable(new FMessagePacket(MessageId));

		if (!FStructNetSerializer::MSerialize(MsgStructAddr, *MsgStructProperty->Struct, *messageWriter.Get()))
		{
			UE_LOG(LogNetPeer, Error, TEXT("Serialize message %d to failed!\n%s:%s"), MessageId, *MsgStructProperty->GetName(), *MsgStructProperty->Struct->GetName());
			return;
		}

		if (messageWriter->GetRawNum() >= MaxMessageLength)
		{
			UE_LOG(LogNetPeer, Error, TEXT("Send failed for the length of message(Id=%d) is too big(%d)!\n%s:%s"), MessageId, messageWriter->GetRawNum(), *MsgStructProperty->GetName(), *MsgStructProperty->Struct->GetName());
			*(bool*)Z_Param__Result = false;
			return;
		}

		messageWriter->FinishWrite();

		Outbox.Enqueue(messageWriter);

		*(bool*)Z_Param__Result = true;

		P_NATIVE_END
	}
	
	/**
	 * Connect to the server. for the call from C++ only.
	 *
	 * @param ServerIpAndPort		The ip and port of connect to server.  See 
	 **/
	bool Connect(const FString& ServerIpAndPort);
		
	/**
	* Disconnect the connection.
	**/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp")
	void Disconnect();
	
	/**
	 * Gets the human readable name of the connection.
	 *
	 * @return Connection name.
	 **/
	UFUNCTION(BlueprintCallable, Category = "NetPeer|Tcp")
	FString GetName() const;

protected:
	/**
	 * Bind the callback function for a message.
	 *
	 * @param MessageId		The id of message.
	 * @param pfnCallback	The callback function. If pfnCallback return false, the bind will be auto removed.
	 **/
	void BindMessageHandler(FMessageIdType MessageId, const FMessageHeadlerFunction& pfnCallback);

	/**
	 * Bind the callback function for a message. note: without message body.
	 *
	 * @param MessageId		The id of message.
	 * @param pfnCallback	The callback function.
	 **/
	void BindMessageHandler_0(FMessageIdType MessageId, const TFunction<void ()>& pfnCallback);

	/**
	 * Rebind the callback function for private message.
	 *
	 * @param MessageId		The id of message.
	 * @param pfnCallback	The callback function.
	 * @return				The old callback function.
	 **/
	FMessageHeadlerFunction RebindPrivateMessage(FMessageIdType MessageId, const TFunction<void ()>& pfnCallback);

	/**
	 * Resume the exec of blueprint function.
	 *
	 * @param CallbackTargetWeakPtr The Object of blueprint function in.
	 * @param strExecutionFunction The function to resume exec.
	 * @param LinkID The resume point within the function to execute
	 * check FLatentActionInfo for detail.
	 * @return true for Succeed, false for failed.
	 **/
	bool ResumeBlueprintExec(const FWeakObjectPtr& CallbackTargetWeakPtr, const FName& strExecutionFunction, int32 LinkID);
	
protected:

	/**
	 * Callback per frame.
	 **/
	bool Tick(float DeltaTime);
		
	/**
	 * Reconnect to server.
	 *
	 * @param Endpoint The address of remote.
	 **/
	bool ReConnect(const FString& ServerIpAndPort);

	/**
	* Updates connection state and notifies delegate
	**/
	void UpdateConnectionState(EConnectionState NewConnectionState);

private:

// #pragma region("In working thread")  // XCode not support
	//** -------------------------------------------------------------------------------------------------------------

	/**
	* Receives all pending messages from the socket.
	*
	* @return true on success, false otherwise.
	* @see SendMessages
	**/
	bool ReceiveMessages();

	/**
	* Sends all pending messages to the socket.
	*
	* @return true on success, false otherwise.
	* @see ReceiveMessages
	**/
	bool SendMessages();


	/** Try to send data, but if all data is not sent in one go, block on send until data is sent or an error occurs */
	bool BlockingSend(const uint8* Data, int32 BytesToSend);

	/**
	 * Check connecting state. Called in back thread.
	 *
	 * @return true for connect successed, false for failed.
	 **/
	bool CheckConnecting();

	/**
	 * Try to reconnect. Called in back thread.
	 **/
	bool TryReconnect();

// #pragma endregion

private:

	//~ FRunnable interface

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	float ConnectingTime;			// The max time of connect.(second)

	FDelegateHandle TickHandler;	// FTick's handler

private:
	FSocket*	pSocket;			// tcp instance.
	
	/** Holds the IP and port of the server. See: FInternetAddrBSDIPv6::SetIp(const TCHAR* InAddr, bool& bIsValid) comment
	 * IPv6 - [1111:2222:3333:4444:5555:6666:7777:8888]:PORT || [1111:2222:3333::]:PORT || [::ffff:IPv4]:PORT
	 * IPv4 - aaa.bbb.ccc.ddd:PORT || 127.0.0.1:1234:PORT
	 **/
	FString m_strServerIpAndPort;

	/** Holds the thread object. */
	FRunnableThread* Thread;

	/** Holds the time at which the connection was opened. */
	FDateTime OpenedTime;

	/** Holds the time at which the connection was closed. */
	FDateTime ClosedTime;

	/** thread should continue running. */
	bool bRun;

	/** Current connection state */
	EConnectionState ConnectionState;

	/** Whether we've sent the initial header to the remote end */
	bool bSentHeader;

	/** Whether we've received the initial header from the remote end */
	bool bReceivedHeader;

	/** Holds the total number of bytes received from the connection. */
	uint64 TotalBytesReceived;

	/** Holds the total number of bytes sent to the connection. */
	uint64 TotalBytesSent;

	/** Retry times before re-establishing connection if it drops, 0 disables */
	UPROPERTY(EditAnywhere)
	int32 ConnectionRetryTimes;

	/** Message data we're currently in the process of receiving, if any */
	FMessagePacketPtr RecvMessageData;

	/** The number of bytes of incoming message data we're still waiting on before we have a complete message */
	FMessageLenthType RecvMessageDataRemaining;

	/** Holds the collection of received Messages. */
	TQueue<FMessagePacketPtr, EQueueMode::Spsc> Inbox;

	/** Holds the collection of outbound messages. */
	TQueue<FMessageWriterPtr, EQueueMode::Spsc> Outbox;

	// The map of message id and the callback function.
	TMap< FMessageIdType, FMessageHeadlerFunction > MessageHandles;
};
