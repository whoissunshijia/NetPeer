// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once

// Include the mpack's cpp for easy different platform's build.
#include "ThirdParty/mpackLib/mpack-common.h"
#include "ThirdParty/mpackLib/mpack-writer.h"
#include "ThirdParty/mpackLib/mpack-reader.h"
#include "ThirdParty/mpackLib/mpack-platform.h"
#include "ThirdParty/mpackLib/mpack-expect.h"

// forward declarations
class UScriptStruct;
class FArchive;
struct FMessagePacket;

/**
 * Implements a static class that can serialize UStruct based types.
 *
 * This class implements the basic functionality for the serialization of UStructs, such as
 * iterating a structure's properties and writing property values.
 *
 */
class FStructNetSerializer
{
public:

	/**
	 * Serializes a given data structure of the specified type.
	 *
	 * @param StructDataPtr	The structure's data to serialize.
	 * @param TypeInfo	The structure's type information.
	 * @param Steam		The serialization arvhive to use.
	 * @see Deserialize
	 */
	static bool Serialize(const void* StructDataPtr, UScriptStruct& StructTypeInfo, FArchive& Steam );

	/**
	 * Serializes a given USTRUCT.
	 *
	 * @param StructInstance	The struct to serialize.	 
	 * @param Steam				The serialization arvhive to use.
	 */
	template<typename StructInstance>
	static bool Serialize( const StructInstance& Struct, FArchive& Steam)
	{
		return Serialize(&Struct, *Struct.StaticStruct(), Steam);
	}

	/**
	* Deserializes a given data structure of the specified type.
	*
	* @param StructDataPtr	The structure's data to serialize.
	* @param StructTypeInfo	The structure's type information.
	* @param Steam			The serialization arvhive to use.
	* @see Deserialize
	*/
	static bool Deserialize(void* StructDataPtr, UScriptStruct& StructTypeInfo, FArchive& Steam);

	/**
	* Deserializes a given USTRUCT.
	*
	* @param StructInstance	The struct to serialize.	
	* @param Steam			The serialization arvhive to use.
	*/
	template<typename StructInstance>
	static bool Deserialize(StructInstance& Struct, FArchive& Steam)
	{
		return Deserialize(&Struct, *Struct.StaticStruct(), Steam);
	}

	/**
	* Serializes a given data structure of the specified type.
	*
	* @param StructDataPtr	The structure's data to serialize.
	* @param StructTypeInfo	The structure's type information.
	* @param Steam			The serialization arvhive to use.
	* @see Deserialize
	*/
	static bool MSerialize(const void* StructDataPtr, UScriptStruct& StructTypeInfo, FMessagePacket& Steam);

	/**
	* Serializes a given USTRUCT.
	*
	* @param StructInstance	The struct to serialize.
	* @param Steam			The serialization arvhive to use.
	*/
	template<typename StructInstance>
	static bool MSerialize(const StructInstance& Struct, FMessagePacket& Steam)
	{
		return MSerialize(&Struct, *Struct.StaticStruct(), Steam);
	}

	/**
	* Deserializes a given data structure of the specified type.
	*
	* @param StructDataPtr	The structure's data to serialize.
	* @param StructTypeInfo	The structure's type information.
	* @param Data			The data to read from.
	* @param len			The data's length.
	* @see Deserialize
	*/
	static bool MDeserialize(void* StructDataPtr, UScriptStruct& StructTypeInfo, const uint8* Data, uint32 len);

	/**
	* Deserializes a given USTRUCT.
	*
	* @param StructInstance	The struct to serialize.
	* @param Data			The data to read from.
	* @param len			The data's length.
	*/
	template<typename StructInstance>
	static bool MDeserialize(StructInstance& Struct, const uint8* Data, uint32 len)
	{
		return MDeserialize(&Struct, *Struct.StaticStruct(), Data, len);
	}

#if WITH_MSGPACK_CPLUSPLUS	// Not used for exception is not support in android.
	/**
	* Serializes a given data structure of the specified type.
	*
	* @param StructDataPtr	The structure's data to serialize.
	* @param StructTypeInfo	The structure's type information.
	* @param Steam			The serialization arvhive to use.
	* @see Deserialize
	*/
	static bool Serialize(const void* StructDataPtr, UScriptStruct& StructTypeInfo, FMessagePacket& Steam);

	/**
	* Serializes a given USTRUCT.
	*
	* @param StructInstance	The struct to serialize.	
	* @param Steam			The serialization arvhive to use.
	*/
	template<typename StructInstance>
	static bool Serialize(const StructInstance& Struct, FMessagePacket& Steam)
	{
		return Serialize(&Struct, *Struct.StaticStruct(), Steam);
	}

	/**
	* Deserializes a given data structure of the specified type.
	*
	* @param StructDataPtr	The structure's data to serialize.
	* @param StructTypeInfo	The structure's type information.
	* @param Data			The data to read from.
	* @param len			The data's length.
	* @see Deserialize
	*/
	static bool Deserialize(void* StructDataPtr, UScriptStruct& StructTypeInfo, const uint8* Data, uint32 len);

	/**
	* Deserializes a given USTRUCT.
	*
	* @param StructInstance	The struct to serialize.	
	* @param Data			The data to read from.
	* @param len			The data's length.
	*/
	template<typename StructInstance>
	static bool Deserialize(StructInstance& Struct, const uint8* Data, uint32 len)
	{
		return Deserialize(&Struct, *Struct.StaticStruct(), Data, len);
	}
#endif
	
};
