// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#include "NetPeerPrivatePCH.h"
#include "AutomationTest.h"
#include "../Net/StructNetSerializer.h"
#include "NetStructSerializerTestTypes.h"
#include "../Net/TcpNetPeer.h"

#if WITH_DEV_AUTOMATION_TESTS

/* Internal helpers
 *****************************************************************************/

namespace StructSerializerTest
{
#if WITH_MSGPACK_CPLUSPLUS
	template<typename T>
	struct FStructByteArray_Raw
	{
		std::vector<T> int8Array;

		FNetPeerStructSerializerNumericTestStruct Numberic;

		FStructByteArray_Raw()
		{
			int8Array.push_back(1);
			int8Array.push_back(2);
			int8Array.push_back(3);
		}

		MSGPACK_DEFINE(int8Array, Numberic);
	};

	template<typename T>
	struct FStructCArray_Raw
	{
		T int8Array[3];

		FStructCArray_Raw()
		{
			int8Array[0] = 2;
			int8Array[1] = 3;
			int8Array[2] = 4;
		}

		MSGPACK_DEFINE(int8Array);
	};
		
	struct FStructBuildin
	{
		int16 int16Data;
		std::string Name;
		std::string String;
		std::string Text;

		FStructBuildin() :
			int16Data(0),
			Name("This is a FName"),
			String("Test String")			
			, Text("Test Text")
		{
		}

		MSGPACK_DEFINE(int16Data, Name, String, Text);
	};
#endif

	void TestSerialization( FAutomationTestBase& Test, FMemoryWriter& SerializerBackend, FMemoryReader& DeserializerBackend )
	{
		// serialization
		FNetPeerStructSerializerTestStruct TestStruct;
		{
			Test.TestTrue(TEXT("Deserialization must succeed"), FStructNetSerializer::Serialize(TestStruct, SerializerBackend));
		}

		// deserialization
		FNetPeerStructSerializerTestStruct TestStruct2(NoInit);
		{			
			Test.TestTrue(TEXT("Deserialization must succeed"), FStructNetSerializer::Deserialize(TestStruct2, DeserializerBackend));
		}

		// test numerics
		Test.TestEqual<int8>(TEXT("Numerics.Int8 value must be the same before and after de-/serialization"), TestStruct.Numerics.Int8, TestStruct2.Numerics.Int8);
		Test.TestEqual<int16>(TEXT("Numerics.Int16 value must be the same before and after de-/serialization"), TestStruct.Numerics.Int16, TestStruct2.Numerics.Int16);
		Test.TestEqual<int32>(TEXT("Numerics.Int32 value must be the same before and after de-/serialization"), TestStruct.Numerics.Int32, TestStruct2.Numerics.Int32);
		Test.TestEqual<int64>(TEXT("Numerics.Int64 value must be the same before and after de-/serialization"), TestStruct.Numerics.Int64, TestStruct2.Numerics.Int64);
		Test.TestEqual<uint8>(TEXT("Numerics.UInt8 value must be the same before and after de-/serialization"), TestStruct.Numerics.UInt8, TestStruct2.Numerics.UInt8);
		Test.TestEqual<uint16>(TEXT("Numerics.UInt16 value must be the same before and after de-/serialization"), TestStruct.Numerics.UInt16, TestStruct2.Numerics.UInt16);
		Test.TestEqual<uint32>(TEXT("Numerics.UInt32 value must be the same before and after de-/serialization"), TestStruct.Numerics.UInt32, TestStruct2.Numerics.UInt32);
		Test.TestEqual<uint64>(TEXT("Numerics.UInt64 value must be the same before and after de-/serialization"), TestStruct.Numerics.UInt64, TestStruct2.Numerics.UInt64);
		Test.TestEqual<float>(TEXT("Numerics.Float value must be the same before and after de-/serialization"), TestStruct.Numerics.Float, TestStruct2.Numerics.Float);
		Test.TestEqual<double>(TEXT("Numerics.Double value must be the same before and after de-/serialization"), TestStruct.Numerics.Double, TestStruct2.Numerics.Double);

		// test booleans
		Test.TestEqual<bool>(TEXT("Booleans.BoolFalse must be the same before and after de-/serialization"), TestStruct.Booleans.BoolFalse, TestStruct2.Booleans.BoolFalse);
		Test.TestEqual<bool>(TEXT("Booleans.BoolTrue must be the same before and after de-/serialization"), TestStruct.Booleans.BoolTrue, TestStruct2.Booleans.BoolTrue);
		Test.TestEqual<uint32>(TEXT("Booleans.Bitfield must be the same before and after de-/serialization"), TestStruct.Booleans.Bitfield, TestStruct2.Booleans.Bitfield);

		// test objects
		// Test.TestEqual<TSubclassOf<class UObject>>(TEXT("Objects.Class must be the same before and after de-/serialization"), TestStruct.Objects.Class, TestStruct2.Objects.Class);
		// Test.TestEqual<UObject*>(TEXT("Objects.ObjectPtr must be the same before and after de-/serialization"), TestStruct.Objects.ObjectPtr, TestStruct2.Objects.ObjectPtr);

		// test built-ins
		// Test.TestEqual<FGuid>(TEXT("Builtins.Guid must be the same before and after de-/serialization"), TestStruct.Builtins.Guid, TestStruct2.Builtins.Guid);
		Test.TestEqual<FName>(TEXT("Builtins.Name must be the same before and after de-/serialization"), TestStruct.Builtins.Name, TestStruct2.Builtins.Name);
		Test.TestEqual<FString>(TEXT("Builtins.String must be the same before and after de-/serialization"), TestStruct.Builtins.String, TestStruct2.Builtins.String);
		// Test.TestEqual<FRotator>(TEXT("Builtins.Rotator must be the same before and after de-/serialization"), TestStruct.Builtins.Rotator, TestStruct2.Builtins.Rotator);
		Test.TestEqual<FString>(TEXT("Builtins.Text must be the same before and after de-/serialization"), TestStruct.Builtins.Text.ToString(), TestStruct2.Builtins.Text.ToString());
		// Test.TestEqual<FVector>(TEXT("Builtins.Vector must be the same before and after de-/serialization"), TestStruct.Builtins.Vector, TestStruct2.Builtins.Vector);

		// test arrays
		Test.TestEqual<TArray<int32>>(TEXT("Arrays.Int32Array must be the same before and after de-/serialization"), TestStruct.Arrays.Int32Array, TestStruct2.Arrays.Int32Array);
		Test.TestEqual<int32>(TEXT("Arrays.StaticSingleElement[0] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticSingleElement[0], TestStruct2.Arrays.StaticSingleElement[0]);
		Test.TestEqual<int32>(TEXT("Arrays.StaticInt32Array[0] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticInt32Array[0], TestStruct2.Arrays.StaticInt32Array[0]);
		Test.TestEqual<int32>(TEXT("Arrays.StaticInt32Array[1] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticInt32Array[1], TestStruct2.Arrays.StaticInt32Array[1]);
		Test.TestEqual<int32>(TEXT("Arrays.StaticInt32Array[2] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticInt32Array[2], TestStruct2.Arrays.StaticInt32Array[2]);
		Test.TestEqual<float>(TEXT("Arrays.StaticFloatArray[0] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticFloatArray[0], TestStruct2.Arrays.StaticFloatArray[0]);
		Test.TestEqual<float>(TEXT("Arrays.StaticFloatArray[1] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticFloatArray[1], TestStruct2.Arrays.StaticFloatArray[1]);
		Test.TestEqual<float>(TEXT("Arrays.StaticFloatArray[2] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticFloatArray[2], TestStruct2.Arrays.StaticFloatArray[2]);
		Test.TestEqual<TArray<FNetPeerStructSerializerBooleanTestStruct>>(TEXT("Arrays.StructArray must be the same before and after de-/serialization"), TestStruct.Arrays.StructArray, TestStruct2.Arrays.StructArray);
		//Test.TestEqual<FStructSerializerBuiltinTestStruct>(TEXT("Arrays.StaticStructArray[0] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticStructArray[0], TestStruct2.Arrays.StaticStructArray[0]);
		Test.TestEqual<FNetPeerStructSerializerBuiltinTestStruct>(TEXT("Arrays.StaticStructArray[1] must be the same before and after de-/serialization"), TestStruct.Arrays.StaticStructArray[1], TestStruct2.Arrays.StaticStructArray[1]);

		// Test.TestEqual<TArray<FVector>>(TEXT("Arrays.VectorArray must be the same before and after de-/serialization"), TestStruct.Arrays.VectorArray, TestStruct2.Arrays.VectorArray);

		// test maps
		// Test.TestTrue(TEXT("Maps.IntToStr must be the same before and after de-/serialization"), TestStruct.Maps.IntToStr.OrderIndependentCompareEqual(TestStruct2.Maps.IntToStr));
		// Test.TestTrue(TEXT("Maps.StrToStr must be the same before and after de-/serialization"), TestStruct.Maps.StrToStr.OrderIndependentCompareEqual(TestStruct2.Maps.StrToStr));
		// Test.TestTrue(TEXT("Maps.StrToVec must be the same before and after de-/serialization"), TestStruct.Maps.StrToVec.OrderIndependentCompareEqual(TestStruct2.Maps.StrToVec));
#if WITH_MSGPACK_CPLUSPLUS
		{
			FMessagePacket buffer;
			FNetPeerStructByteArray ByteArray;
			FStructNetSerializer::Serialize(ByteArray, buffer);
						
			FMessagePacket Mbuffer;
			FStructNetSerializer::MSerialize(ByteArray, Mbuffer);

			FNetPeerStructByteArray ByteArrayDes;
			FStructNetSerializer::Deserialize(ByteArrayDes, buffer.GetRawData(), buffer.GetRawNum());

			FNetPeerStructByteArray ByteArrayDes_M;
			FStructNetSerializer::MDeserialize(ByteArrayDes_M, buffer.GetRawData(), buffer.GetRawNum());

			FStructByteArray_Raw<int8> ByteArray_int8;
			FMessagePacket buffer2;
			msgpack::pack(buffer2, ByteArray_int8);

			Test.TestTrue(TEXT("1. FStructByteArray must be the same."), FMemory::Memcmp(buffer.GetRawData(), buffer2.GetRawData(), buffer.GetRawNum()) == 0);
			Test.TestTrue(TEXT("2. FStructByteArray must be the same."), FMemory::Memcmp(buffer.GetRawData(), Mbuffer.GetRawData(), buffer.GetRawNum()) == 0 && buffer.GetRawNum() == Mbuffer.GetRawNum());
			Test.TestEqual<TArray<int8>>(TEXT("3. FStructByteArray.int8Array must be the same before and after de-/serialization"), ByteArray.int8Array, ByteArrayDes.int8Array);
			Test.TestEqual<TArray<int8>>(TEXT("4. FStructByteArray.int8Array must be the same before and after de-/MSerialization"), ByteArray.int8Array, ByteArrayDes_M.int8Array);
			Test.TestEqual(TEXT("5. FStructByteArray.Numberic must be the same before and after de-/serialization"), ByteArray.Numberic, ByteArrayDes.Numberic);
			Test.TestEqual(TEXT("6. FStructByteArray.Numberic must be the same before and after de-/Mserialization"), ByteArray.Numberic, ByteArrayDes_M.Numberic);
		}
		{
			FMessagePacket buffer;
			FNetPeerStructArray_char ByteArray;
			FStructNetSerializer::Serialize(ByteArray, buffer);

			FMessagePacket Mbuffer;
			FStructNetSerializer::MSerialize(ByteArray, Mbuffer);

			FNetPeerStructArray_char ByteArrayDes;
			FStructNetSerializer::Deserialize(ByteArrayDes, buffer.GetRawData(), buffer.GetRawNum());

			FNetPeerStructArray_char ByteArrayDes_M;
			FStructNetSerializer::MDeserialize(ByteArrayDes_M, buffer.GetRawData(), buffer.GetRawNum());

			FStructByteArray_Raw<uint8> ByteArray_Byte;
			FMessagePacket buffer2;
			msgpack::pack(buffer2, ByteArray_Byte);

			Test.TestTrue(TEXT("1. FStructArray_char must be the same."), (buffer.GetRawNum() == buffer2.GetRawNum()) && (FMemory::Memcmp(buffer.GetRawData(), buffer2.GetRawData(), buffer.GetRawNum()) == 0));
			Test.TestTrue(TEXT("2. FStructArray_char must be the same."), FMemory::Memcmp(buffer.GetRawData(), Mbuffer.GetRawData(), buffer.GetRawNum()) == 0 && buffer.GetRawNum() == Mbuffer.GetRawNum());
			Test.TestEqual<TArray<uint8>>(TEXT("3. FStructArray_char.int8Array must be the same before and after de-/serialization"), ByteArray.int8Array, ByteArrayDes.int8Array);
			Test.TestEqual<TArray<uint8>>(TEXT("4. FStructArray_char.int8Array must be the same before and after de-/serialization"), ByteArray.int8Array, ByteArrayDes_M.int8Array);
		}

		{
			FMessagePacket buffer;
			FNetPeerStructCArray_UnsignedChar ByteArray;
			FStructNetSerializer::Serialize(ByteArray, buffer);

			FMessagePacket Mbuffer;
			FStructNetSerializer::MSerialize(ByteArray, Mbuffer);

			FNetPeerStructCArray_UnsignedChar ByteArrayDes;
			FStructNetSerializer::Deserialize(ByteArrayDes, buffer.GetRawData(), buffer.GetRawNum());

			FNetPeerStructCArray_UnsignedChar ByteArrayDes_M;
			FStructNetSerializer::MDeserialize(ByteArrayDes_M, buffer.GetRawData(), buffer.GetRawNum());

			FStructCArray_Raw<uint8> ByteArray_Byte;
			FMessagePacket buffer2;
			msgpack::pack(buffer2, ByteArray_Byte);

			Test.TestTrue(TEXT("1. FStructCArray_UnsignedChar must be the same."), (buffer.GetRawNum() == buffer2.GetRawNum()) && (FMemory::Memcmp(buffer.GetRawData(), buffer2.GetRawData(), buffer.GetRawNum()) == 0) );
			Test.TestTrue(TEXT("2. FStructCArray_UnsignedChar must be the same."), FMemory::Memcmp(buffer.GetRawData(), Mbuffer.GetRawData(), buffer.GetRawNum()) == 0 && buffer.GetRawNum() == Mbuffer.GetRawNum());
			Test.TestTrue(TEXT("3. FStructCArray_UnsignedChar.int8Array must be the same before and after de-/serialization."), FMemory::Memcmp(ByteArrayDes.int8Array, ByteArray.int8Array, ARRAY_COUNT(ByteArray.int8Array)) == 0);
			Test.TestTrue(TEXT("4. FStructCArray_UnsignedChar.int8Array must be the same before and after de-/Mserialization."), FMemory::Memcmp(ByteArrayDes_M.int8Array, ByteArray.int8Array, ARRAY_COUNT(ByteArray.int8Array)) == 0);
		}
		{
			FMessagePacket buffer;
			FNetPeerStructSerializerBuiltinTestStruct BuiltinTest;
			FStructNetSerializer::Serialize(BuiltinTest, buffer);

			FMessagePacket Mbuffer;
			FStructNetSerializer::MSerialize(BuiltinTest, Mbuffer);

			FNetPeerStructSerializerBuiltinTestStruct BuiltinTestDes;
			FStructNetSerializer::Deserialize(BuiltinTestDes, buffer.GetRawData(), buffer.GetRawNum());

			FNetPeerStructSerializerBuiltinTestStruct BuiltinTestDes_M;
			FStructNetSerializer::Deserialize(BuiltinTestDes_M, buffer.GetRawData(), buffer.GetRawNum());

			FStructBuildin BuiltinTest_C;
			FMessagePacket buffer2;
			msgpack::pack(buffer2, BuiltinTest_C);

			Test.TestTrue(TEXT("1. FStructSerializerBuiltinTestStruct must be the same."), (buffer.GetRawNum() == buffer2.GetRawNum()) && (FMemory::Memcmp(buffer.GetRawData(), buffer2.GetRawData(), buffer.GetRawNum()) == 0) );
			Test.TestTrue(TEXT("2. FStructSerializerBuiltinTestStruct must be the same."), FMemory::Memcmp(buffer.GetRawData(), Mbuffer.GetRawData(), buffer.GetRawNum()) == 0 && buffer.GetRawNum() == Mbuffer.GetRawNum());

			Test.TestEqual<FName>(TEXT("3. BuiltinTest.Name must be the same before and after de-/serialization"), BuiltinTest.Name, BuiltinTestDes.Name);
			Test.TestEqual<FString>(TEXT("4. BuiltinTest.String must be the same before and after de-/serialization"), BuiltinTest.String, BuiltinTestDes.String);
			Test.TestEqual<FString>(TEXT("5. BuiltinTest.Text must be the same before and after de-/serialization"), BuiltinTest.Text.ToString(), BuiltinTestDes.Text.ToString());
			Test.TestEqual<FAInt16>(TEXT("6. BuiltinTest.Text must be the same before and after de-/serialization"), BuiltinTest.int16Data, BuiltinTestDes.int16Data);

			Test.TestEqual<FName>(TEXT("7. BuiltinTest.Name must be the same before and after de-/serialization"), BuiltinTest.Name, BuiltinTestDes_M.Name);
			Test.TestEqual<FString>(TEXT("8. BuiltinTest.String must be the same before and after de-/serialization"), BuiltinTest.String, BuiltinTestDes_M.String);
			Test.TestEqual<FString>(TEXT("9. BuiltinTest.Text must be the same before and after de-/serialization"), BuiltinTest.Text.ToString(), BuiltinTestDes_M.Text.ToString());
			Test.TestEqual<FAInt16>(TEXT("10. BuiltinTest.Text must be the same before and after de-/serialization"), BuiltinTest.int16Data, BuiltinTestDes_M.int16Data);
		}
#endif	// WITH_MSGPACK_CPLUSPLUS
	}
}

/* Tests
 *****************************************************************************/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNetStructSerializerTest, "NetPeer.Net.NetStructSerializer", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


bool FNetStructSerializerTest::RunTest( const FString& Parameters )
{
	// json
	{
		TArray<uint8> Buffer;
		FMemoryReader Reader(Buffer);
		FMemoryWriter Writer(Buffer);

		StructSerializerTest::TestSerialization(*this, Writer, Reader);

		// uncomment this to look at the serialized data
//		GLog->Logf(TEXT("%s"), (TCHAR*)Buffer.GetData());
	}

	return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS