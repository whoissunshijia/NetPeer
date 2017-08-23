// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#if WITH_MSGPACK_CPLUSPLUS
#include "msgpack.hpp"
#endif
#include "UE4FundamentalTypeWrapper/Int16.h"
#include "NetStructSerializerTestTypes.generated.h"

/**
 * Test structure for numeric properties.
 */
USTRUCT()
struct FNetPeerStructSerializerNumericTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int8 Int8;

	UPROPERTY()
	int16 Int16;

	UPROPERTY()
	int32 Int32;

	UPROPERTY()
	int64 Int64;

	UPROPERTY()
	uint8 UInt8;

	UPROPERTY()
	uint16 UInt16;

	UPROPERTY()
	uint32 UInt32;

	UPROPERTY()
	uint64 UInt64;

	UPROPERTY()
	float Float;

	UPROPERTY()
	double Double;

	bool operator==(const FNetPeerStructSerializerNumericTestStruct& right) const
	{
		return Int8 == right.Int8 && Int16 == right.Int16 && Int32 == right.Int32 && Int64 == right.Int64
			&& UInt8 == right.UInt8 && UInt16 == right.UInt16 && UInt32 == right.UInt32 && UInt64 == right.UInt64
			&& Float == right.Float && Double == right.Double;
	}

	bool operator!=(const FNetPeerStructSerializerNumericTestStruct& right) const
	{
		return !(*this == right);
	}

	/** Default constructor. */
	FNetPeerStructSerializerNumericTestStruct()
		: Int8(-127)
		, Int16(-32767)
		, Int32(-2147483647)
		, Int64(-92233720368547/*75807*/)
		, UInt8(255)
		, UInt16(65535)
		, UInt32(4294967295)
		, UInt64(18446744073709/*551615*/)
		, Float(4.125)
		, Double(1.03125)
	{ }

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerNumericTestStruct( ENoInit ) { }

#if WITH_MSGPACK_CPLUSPLUS
	MSGPACK_DEFINE(Int8, Int16, Int32, Int64, UInt8, UInt16, UInt32, UInt64, Float, Double)
#endif
};


/**
 * Test structure for boolean properties.
 */
USTRUCT()
struct FNetPeerStructSerializerBooleanTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool BoolFalse;

	UPROPERTY()
	bool BoolTrue;

	UPROPERTY()
	uint32 Bitfield;

	bool operator==(const FNetPeerStructSerializerBooleanTestStruct& right) const
	{
		return BoolFalse == right.BoolFalse && BoolTrue == right.BoolTrue && Bitfield == right.Bitfield;
	}

	bool operator!=(const FNetPeerStructSerializerBooleanTestStruct& right) const
	{
		return !(*this == right);
	}

	/** Default constructor. */
	FNetPeerStructSerializerBooleanTestStruct()
		: BoolFalse(false)
		, BoolTrue(true)
		, Bitfield(1)
	{ }

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerBooleanTestStruct( ENoInit ) { }
};


/**
 * Test structure for UObject properties.
 */
USTRUCT()
struct FNetPeerStructSerializerObjectTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TSubclassOf<class UObject> Class;

	UPROPERTY()
	class UObject* ObjectPtr;

	/** Default constructor. */
	FNetPeerStructSerializerObjectTestStruct()
		: ObjectPtr(nullptr)
	{ }

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerObjectTestStruct( ENoInit ) { }
};


/**
 * Test structure for properties of various built-in types.
 */
USTRUCT()
struct FNetPeerStructSerializerBuiltinTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FAInt16 int16Data;
	// UPROPERTY()
	// FGuid Guid;

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FString String;

	// UPROPERTY()
	// FRotator Rotator;

	UPROPERTY()
	FText Text;
	
	bool operator==(const FNetPeerStructSerializerBuiltinTestStruct& right) const
	{
		return Name == right.Name && String == right.String && Text.ToString() == right.Text.ToString() && int16Data.Value == int16Data.Value;
	}

	bool operator!=(const FNetPeerStructSerializerBuiltinTestStruct& right) const
	{
		return !(*this == right);
	}
	// UPROPERTY()
	// FVector Vector;

	/** Default constructor. */
	FNetPeerStructSerializerBuiltinTestStruct()
		: //Guid(FGuid::NewGuid()), 
		Name("This is a FName"),
		String("Test String")
		//, Rotator(4096, 8192, 16384)
		, Text(FText::FromString("Test Text"))
		//, Vector(1.0f, 2.0f, 3.0f)		
	{ }

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerBuiltinTestStruct( ENoInit ) { }
};

/**
 * Test structure for array properties.
 */
USTRUCT()
struct FNetPeerStructSerializerArrayTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<int32> Int32Array;

	UPROPERTY()
	TArray<FNetPeerStructSerializerBooleanTestStruct> StructArray;

	UPROPERTY()
	FNetPeerStructSerializerBuiltinTestStruct StaticStructArray[2];

	UPROPERTY()
	int32 StaticSingleElement[1];

	UPROPERTY()
	int32 StaticInt32Array[3];

	UPROPERTY()
	float StaticFloatArray[3];

	//UPROPERTY()
	//TArray<FVector> VectorArray;

	/** Default constructor. */
	FNetPeerStructSerializerArrayTestStruct()
	{
		Int32Array.Add(-1);
		Int32Array.Add(0);
		Int32Array.Add(1);

		StaticSingleElement[0] = 42;

		StaticInt32Array[0] = -1;
		StaticInt32Array[1] = 0;
		StaticInt32Array[2] = 1;

		StaticFloatArray[0] = -1.0f;
		StaticFloatArray[1] = 0.0f;
		StaticFloatArray[2] = 1.0f;

//		VectorArray.Add(FVector(1.0f, 2.0f, 3.0f));
//		VectorArray.Add(FVector(-1.0f, -2.0f, -3.0f));
	}

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerArrayTestStruct( ENoInit ) { }
};


/**
 * Test structure for map properties.
 */
USTRUCT()
struct FNetPeerStructSerializerMapTestStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TMap<int32, FString> IntToStr;

	UPROPERTY()
	TMap<FString, FString> StrToStr;

	UPROPERTY()
	TMap<FString, FVector> StrToVec;

	/** Default constructor. */
	FNetPeerStructSerializerMapTestStruct()
	{
		IntToStr.Add(1, TEXT("One"));
		IntToStr.Add(2, TEXT("Two"));
		IntToStr.Add(3, TEXT("Three"));

		StrToStr.Add(TEXT("StrAll"), TEXT("All"));
		StrToStr.Add(TEXT("StrYour"), TEXT("Your"));
		StrToStr.Add(TEXT("StrBase"), TEXT("Base"));

		StrToVec.Add(TEXT("V000"), FVector(0.0f, 0.0f, 0.0f));
		StrToVec.Add(TEXT("V123"), FVector(1.0f, 2.0f, 3.0f));
		StrToVec.Add(TEXT("V666"), FVector(6.0f, 6.0f, 6.0f));
	}

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerMapTestStruct( ENoInit ) { }
};


/**
 * Test structure for all supported types.
 */
USTRUCT()
struct FNetPeerStructSerializerTestStruct
{
	GENERATED_USTRUCT_BODY()

 	UPROPERTY()
 	FNetPeerStructSerializerNumericTestStruct Numerics;
 
 	UPROPERTY()
 	FNetPeerStructSerializerBooleanTestStruct Booleans;

	// UPROPERTY()
	// FStructSerializerObjectTestStruct Objects;

 	UPROPERTY()
 	FNetPeerStructSerializerBuiltinTestStruct Builtins;

	UPROPERTY()
	FNetPeerStructSerializerArrayTestStruct Arrays;

	// UPROPERTY()
	// FStructSerializerMapTestStruct Maps;

	/** Default constructor. */
	FNetPeerStructSerializerTestStruct() { }

	/** Creates an uninitialized instance. */
	FNetPeerStructSerializerTestStruct( ENoInit )
		: 
		// Numerics(NoInit)
		//, Booleans(NoInit)
		//, Objects(NoInit)
		//, Builtins(NoInit),
		Arrays(NoInit)
		//, Maps(NoInit)
	{ }
};

USTRUCT()
struct FNetPeerStructByteArray
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<int8> int8Array;

	UPROPERTY()
	FNetPeerStructSerializerNumericTestStruct Numberic;

	FNetPeerStructByteArray()
	{
		int8Array.Add(1);
		int8Array.Add(2);
		int8Array.Add(3);
	}
};

USTRUCT()
struct FNetPeerStructArray_char
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<uint8> int8Array;

	UPROPERTY()
	FNetPeerStructSerializerNumericTestStruct Numberic;

	FNetPeerStructArray_char()
	{
		int8Array.Add(1);
		int8Array.Add(2);
		int8Array.Add(3);
	}
};

USTRUCT()
struct FNetPeerStructCArray_UnsignedChar
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint8 int8Array[3];

	FNetPeerStructCArray_UnsignedChar()
	{
		int8Array[0] = 2;
		int8Array[1] = 3;
		int8Array[2] = 4;
	}
};