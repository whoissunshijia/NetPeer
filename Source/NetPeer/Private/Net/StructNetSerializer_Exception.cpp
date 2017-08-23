// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#include "NetPeerPrivatePCH.h"
#include "StructNetSerializer.h"
#include "UE4FundamentalTypeWrapper/UE4FundamentalTypeBase.h"
#include "TcpNetPeer.h"

#if WITH_MSGPACK_CPLUSPLUS		// Test only.
#include "msgpack.hpp"

struct FUScriptStructPacker
{
	void* StructDataPtr;
	UScriptStruct* StructTypeInfo;

	FUScriptStructPacker(void* _StructDataPtr, UScriptStruct* _StructTypeInfo)
		: StructDataPtr(_StructDataPtr),
			StructTypeInfo(_StructTypeInfo)
	{
	}
};

struct FUPropertyPacker
{
	void* DataPtr;
	UProperty* Property;

	FUPropertyPacker(void* _DataPtr, UProperty* _Property)
		: Property(_Property),
		DataPtr(_DataPtr)
	{
	}
};

namespace msgpack {
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
		namespace adaptor {

			template<>
			struct convert<FUPropertyPacker> {
				msgpack::object const& operator()(msgpack::object const& msgObj, FUPropertyPacker& DataProperty) const {
					UProperty* Property = DataProperty.Property;
					void* ValuePtr = DataProperty.DataPtr;

					try {

						if (Cast<UNumericProperty>(Property) != nullptr)
						{
							if (Property->GetClass() == UByteProperty::StaticClass())
							{
								msgObj.convert(*(uint8*)ValuePtr);
							}
							else if (Property->GetClass() == UDoubleProperty::StaticClass())
							{
								msgObj.convert(*(double*)ValuePtr);
							}
							else if (Property->GetClass() == UFloatProperty::StaticClass())
							{
								msgObj.convert(*(float*)ValuePtr);
							}
							else if (Property->GetClass() == UIntProperty::StaticClass())
							{
								msgObj.convert(*(int32*)ValuePtr);
							}
							else if (Property->GetClass() == UUInt32Property::StaticClass())
							{
								msgObj.convert(*(uint32*)ValuePtr);
							}
							else if (Property->GetClass() == UInt16Property::StaticClass())
							{
								msgObj.convert(*(int16*)ValuePtr);
							}
							else if (Property->GetClass() == UUInt16Property::StaticClass())
							{
								msgObj.convert(*(uint16*)ValuePtr);
							}
							else if (Property->GetClass() == UInt64Property::StaticClass())
							{
								msgObj.convert(*(int64*)ValuePtr);
							}
							else if (Property->GetClass() == UUInt64Property::StaticClass())
							{
								msgObj.convert(*(uint64*)ValuePtr);
							}
							else if (Property->GetClass() == UInt8Property::StaticClass())
							{
								msgObj.convert(*(int8*)ValuePtr);
							}
							else
							{								
								UE_LOG(LogNetSerialization, Error, TEXT("Numeric field %s (%s) is not supported in UProperty type"), *Property->GetFName().ToString(), *Property->GetClass()->GetName());
								throw msgpack::type_error();
							}
						}
						else if (Property->GetClass() == UBoolProperty::StaticClass())
						{
							msgObj.convert(*(bool*)ValuePtr);
						}
						else if (Property->GetClass() == UStrProperty::StaticClass())
						{
							FString& str = *(FString*)ValuePtr;
							std::string tempString;
							msgObj.convert(tempString);
							str = UTF8_TO_TCHAR(tempString.c_str());
						}
						else if (Property->GetClass() == UNameProperty::StaticClass())
						{
							FName& str = *(FName*)ValuePtr;
							std::string tempString;
							msgObj.convert(tempString);
							str = UTF8_TO_TCHAR(tempString.c_str());
						}
						else if (Property->GetClass() == UTextProperty::StaticClass())
						{
							FText& str = *(FText*)ValuePtr;
							std::string tempString;
							msgObj.convert(tempString);
							str.FromString(UTF8_TO_TCHAR(tempString.c_str()));
						}
						else if (UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Property))
						{
							// We need the helper to get to the items of the array            
							FScriptArrayHelper Helper(ArrayProperty, ValuePtr);

							if (ArrayProperty->Inner->GetClass() == UByteProperty::StaticClass())
							{	// see msgpack/v1/adaptor/vector_unsigned_char.hpp
								switch (msgObj.type)
								{
								case msgpack::type::BIN:
								{
									uint32 arraySize = msgObj.via.bin.size;
									Helper.Resize(arraySize);
									if (arraySize > 0)
									{
										FMemory::Memcpy(Helper.GetRawPtr(0), msgObj.via.bin.ptr, arraySize);
									}
									break;
								}
								case msgpack::type::STR:
								{
									uint32 arraySize = msgObj.via.str.size;
									Helper.Resize(arraySize);
									if (arraySize > 0)
									{
										FMemory::Memcpy(Helper.GetRawPtr(0), msgObj.via.str.ptr, arraySize);
									}
									break;
								}
								default:
									throw msgpack::type_error();
									break;
								}
							}
							else
							{
								if (msgObj.type != msgpack::type::ARRAY)
									throw msgpack::type_error();

								uint32 arraySize = msgObj.via.array.size;
								msgpack::object* PropertyObj = msgObj.via.array.ptr;
								Helper.Resize(arraySize);
								for (uint32 i = 0; i < arraySize; ++i, PropertyObj++)
								{
									FUPropertyPacker _propertyPacker(Helper.GetRawPtr(i), ArrayProperty->Inner);
									
									PropertyObj->convert(_propertyPacker);
								}
								check(PropertyObj == msgObj.via.array.ptr + msgObj.via.array.size);
							}
						}
						else if (UStructProperty* StructProperty = Cast<UStructProperty>(Property))
						{	// Reading a nested struct
							if (StructProperty->Struct->IsChildOf(FRawDataTypeBase::StaticStruct()))
							{
								TFieldIterator<UProperty> It(StructProperty->Struct);
								UProperty* RawDataProperty = *It;								
								// This grabs the pointer to where the property's value is stored.
								void* PropertyValuePtr = RawDataProperty->ContainerPtrToValuePtr<void>(ValuePtr, 0);
								FUPropertyPacker _propertyPacker(PropertyValuePtr, RawDataProperty);
								msgObj.convert(_propertyPacker);
								if (++It, It)
									check(false);	// Only one property is allowed.
							}
							else
							{
								FUScriptStructPacker _structPacker(ValuePtr, StructProperty->Struct);
								msgObj.convert(_structPacker);
							}
						}
						else
						{							
							UE_LOG(LogNetSerialization, Error, TEXT("Property %s (%s) is not supported"), *Property->GetFName().ToString(), *Property->GetClass()->GetName());
							throw std::bad_typeid();
						}

					}
					catch (msgpack::type_error e)
					{
						UE_LOG(LogNetSerialization, Error, TEXT("Property %s (%s) is not same as msgObj type %d"), *Property->GetFName().ToString(), *Property->GetClass()->GetName(), (int32)msgObj.type);
						throw e;
					}
					return msgObj;
				}
			};

			template<>
			struct convert<FUScriptStructPacker> {
				msgpack::object const& operator()(msgpack::object const& msgObj, FUScriptStructPacker& DataStruct) const {
					UScriptStruct* StructTypeInfo = DataStruct.StructTypeInfo;
					void* StructDataPtr = DataStruct.StructDataPtr;

					if (msgObj.type != msgpack::type::ARRAY) { 
						UE_LOG(LogNetSerialization, Error, TEXT("Wrong type (msgObj.type = %d) when convert %s(%s)"), (int32)msgObj.type, *StructTypeInfo->GetFName().ToString(), *StructTypeInfo->GetClass()->GetName());
						throw msgpack::type_error(); 
					}
					
					int32 nPropertyCount = 0;
					// Calc the count of property.
					for (TFieldIterator<UProperty> It(StructTypeInfo); It; ++It, nPropertyCount++);

					if (nPropertyCount != msgObj.via.array.size)
					{
						UE_LOG(LogNetSerialization, Error, TEXT("Wrong size (msgObj properties count=%d, struct properties count=%d) when convert %s(%s)"), msgObj.via.array.size, nPropertyCount, *StructTypeInfo->GetFName().ToString(), *StructTypeInfo->GetClass()->GetName());
						throw msgpack::type_error();
					}

					msgpack::object* propertyObj = msgObj.via.array.ptr;
										
					for (TFieldIterator<UProperty> It(StructTypeInfo); It; ++It, propertyObj++)
					{
						UProperty* Property = *It;

						if (Property->ArrayDim > 1)
						{	// Static array?
							if (Property->GetClass() == UByteProperty::StaticClass())
							{	// See msgpack/v1/adaptor/carray.hpp (struct convert<unsigned char[N]>)
								void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructDataPtr, 0);
								switch (propertyObj->type)
								{
								case msgpack::type::BIN:
									if (propertyObj->via.bin.size != Property->ArrayDim)
									{
										UE_LOG(LogNetSerialization, Error, TEXT("Wrong array's size (propertyObj.size=%d, ArrayDim=%d) when convert %s(%s)"), propertyObj->via.bin.size, Property->ArrayDim, *Property->GetFName().ToString(), *Property->GetClass()->GetName());
										throw msgpack::type_error();
									}
									FMemory::Memcpy(ValuePtr, propertyObj->via.bin.ptr, propertyObj->via.bin.size);
									break;

								case msgpack::type::STR:
									if (propertyObj->via.str.size != Property->ArrayDim)
									{
										UE_LOG(LogNetSerialization, Error, TEXT("Wrong array's size (propertyObj.size=%d, ArrayDim=%d) when convert %s(%s)"), propertyObj->via.str.size, Property->ArrayDim, *Property->GetFName().ToString(), *Property->GetClass()->GetName());
										throw msgpack::type_error();
									}
									FMemory::Memcpy(ValuePtr, propertyObj->via.str.ptr, propertyObj->via.str.size);
									break;
								}
								continue;
							}
						}

						// Never assume ArrayDim is always 1
						for (int32 ArrayIndex = 0; ArrayIndex < Property->ArrayDim; ArrayIndex++)
						{
							// This grabs the pointer to where the property's value is stored.
							void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructDataPtr, ArrayIndex);

							FUPropertyPacker _propertyPacker(ValuePtr, Property);
							propertyObj->convert(_propertyPacker);
						}
					}

					check(propertyObj == (msgObj.via.array.ptr + msgObj.via.array.size));
					
					return msgObj;
				}
			};
						
			template<>
			struct pack<FUPropertyPacker> {
				template<typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& msgObj, FUPropertyPacker const& DataProperty) const {
					UProperty* Property = DataProperty.Property;
					void* ValuePtr = DataProperty.DataPtr;

					if (Cast<UNumericProperty>(Property) != nullptr)
					{
						if (Property->GetClass() == UByteProperty::StaticClass())
						{
							msgObj.pack(*(uint8*)ValuePtr);
						}
						else if (Property->GetClass() == UDoubleProperty::StaticClass())
						{
							msgObj.pack(*(double*)ValuePtr);
						}
						else if (Property->GetClass() == UFloatProperty::StaticClass())
						{
							msgObj.pack(*(float*)ValuePtr);
						}
						else if (Property->GetClass() == UIntProperty::StaticClass())
						{
							msgObj.pack(*(int32*)ValuePtr);
						}
						else if (Property->GetClass() == UUInt32Property::StaticClass())
						{
							msgObj.pack(*(uint32*)ValuePtr);
						}
						else if (Property->GetClass() == UInt16Property::StaticClass())
						{
							msgObj.pack(*(int16*)ValuePtr);
						}
						else if (Property->GetClass() == UUInt16Property::StaticClass())
						{
							msgObj.pack(*(uint16*)ValuePtr);
						}
						else if (Property->GetClass() == UInt64Property::StaticClass())
						{
							msgObj.pack(*(int64*)ValuePtr);
						}
						else if (Property->GetClass() == UUInt64Property::StaticClass())
						{
							msgObj.pack(*(uint64*)ValuePtr);
						}
						else if (Property->GetClass() == UInt8Property::StaticClass())
						{
							msgObj.pack(*(int8*)ValuePtr);
						}
					}
					else if (Property->GetClass() == UBoolProperty::StaticClass())
					{
						msgObj.pack(*(bool*)ValuePtr);
					}
					else if (Property->GetClass() == UStrProperty::StaticClass())
					{
						FString& str = *(FString*)ValuePtr;						
						msgObj.pack(TCHAR_TO_UTF8(*str));
					}
					else if (Property->GetClass() == UNameProperty::StaticClass())
					{
						FName& str = *(FName*)ValuePtr;
						msgObj.pack(TCHAR_TO_UTF8(*str.ToString()));
					}
					else if (Property->GetClass() == UTextProperty::StaticClass())
					{
						FText& str = *(FText*)ValuePtr;
						msgObj.pack(TCHAR_TO_UTF8(*str.ToString()));
					}
					else if (UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Property))
					{
						// We need the helper to get to the items of the array            
						FScriptArrayHelper Helper(ArrayProperty, ValuePtr);

						if (ArrayProperty->Inner->GetClass() == UByteProperty::StaticClass())
						{	// see msgpack/v1/adaptor/vector_unsigned_char.hpp
							uint32 size = checked_get_container_size(Helper.Num());
							msgObj.pack_bin(size);
							if (size > 0)
							{
								msgObj.pack_bin_body((const char*)(Helper.GetRawPtr(0)), size);
							}
						}
						else
						{
							uint32 size = checked_get_container_size(Helper.Num());
							msgObj.pack_array(size);

							for (uint32 i = 0; i < size; ++i)
							{
								msgObj.pack(FUPropertyPacker(Helper.GetRawPtr(i), ArrayProperty->Inner));
							}
						}
					}
					else if (UStructProperty* StructProperty = Cast<UStructProperty>(Property))
					{	
						if (StructProperty->Struct->IsChildOf(FRawDataTypeBase::StaticStruct()))
						{
							TFieldIterator<UProperty> It(StructProperty->Struct);
							UProperty* RawDataProperty = *It;
							// This grabs the pointer to where the property's value is stored.
							void* PropertyValuePtr = RawDataProperty->ContainerPtrToValuePtr<void>(ValuePtr, 0);
							msgObj.pack(FUPropertyPacker(PropertyValuePtr, RawDataProperty));
							if(++It, It)
								check(false);	// Only one property is allowed.
						}
						else
						{// Reading a nested struct
							msgObj.pack(FUScriptStructPacker(ValuePtr, StructProperty->Struct));
						}
					}
					else
					{					
						UE_LOG(LogNetSerialization, Error, TEXT("Property %s (%s) is not supported"), *Property->GetFName().ToString(), *Property->GetClass()->GetName());
						throw msgpack::type_error();
					}
					return msgObj;
				}
			};

			template<>
			struct pack<FUScriptStructPacker> {
				template<typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& msgObj, FUScriptStructPacker const& DataStruct) const {

					int32 nPropertyCount = 0;
					// Calc the count of property.
					for (TFieldIterator<UProperty> It(DataStruct.StructTypeInfo); It; ++It, nPropertyCount++);		

					msgObj.pack_array(nPropertyCount);
					
					for (TFieldIterator<UProperty> It(DataStruct.StructTypeInfo); It; ++It)
					{
						UProperty* Property = *It;

						if (Property->ArrayDim > 1)
						{	// Static Array?
							if (Property->GetClass() == UByteProperty::StaticClass())
							{	// See msgpack/v1/adaptor/carray.hpp (struct convert<unsigned char[N]>)
								uint32 size = checked_get_container_size(Property->ArrayDim);
								msgObj.pack_bin(size);
								const char* ValuePtr = static_cast<const char*>(Property->ContainerPtrToValuePtr<void>(DataStruct.StructDataPtr, 0));
								msgObj.pack_bin_body(ValuePtr, size);
								continue;
							}
							else
								msgObj.pack_array(Property->ArrayDim);
						}

						// Never assume ArrayDim is always 1
						for (int32 ArrayIndex = 0; ArrayIndex < Property->ArrayDim; ArrayIndex++)
						{
							// This grabs the pointer to where the property's value is stored.
							void* ValuePtr = Property->ContainerPtrToValuePtr<void>(DataStruct.StructDataPtr, ArrayIndex);

							msgObj.pack(FUPropertyPacker(ValuePtr, Property));
						}
					}
					return msgObj;

				}
			};
		}
	}
}

bool FStructNetSerializer::Serialize(const void* StructDataPtr, UScriptStruct& StructTypeInfo, FMessagePacket& Steam)
{
	try {
		FUScriptStructPacker _structPacker((void*)StructDataPtr, &StructTypeInfo);
		msgpack::pack(Steam, _structPacker);
		return true;
	}
	catch (...)
	{
		UE_LOG(LogNetSerialization, Error, TEXT("Serialize message struct %s (%s) failed"), *StructTypeInfo.GetFName().ToString(), *StructTypeInfo.GetClass()->GetName());
		return false;
	}

	return true;
}

bool FStructNetSerializer::Deserialize(void* StructDataPtr, UScriptStruct& StructTypeInfo, const uint8* Data, uint32 len)
{
	msgpack::object_handle objHandle = msgpack::unpack((const char*)Data, len);
	const msgpack::object& msgObj = objHandle.get();
	if (msgObj.is_nil())
	{
		UE_LOG(LogNetSerialization, Error, TEXT("Unpack message struct %s (%s) failed"), *StructTypeInfo.GetFName().ToString(), *StructTypeInfo.GetClass()->GetName());
		return false;
	}

	try {
		FUScriptStructPacker _structPacker(StructDataPtr, &StructTypeInfo);
		msgObj.convert(_structPacker);
		return true;
	}
	catch (...)
	{
		UE_LOG(LogNetSerialization, Error, TEXT("Deserialize message struct %s (%s) failed"), *StructTypeInfo.GetFName().ToString(), *StructTypeInfo.GetClass()->GetName());

		const size_t stInfoSize = 4096;
		char szMsgObjInfo[stInfoSize] = { 0 };
		msgpack_object_print_buffer(szMsgObjInfo, stInfoSize, msgObj);
		UE_LOG(LogNetSerialization, Error, TEXT("Msgpack object :(%s)"), ANSI_TO_TCHAR(szMsgObjInfo));

		return false;
	}

	return true;
}

// Print the msgpack object. Debug only.
static int msgpack_object_bin_print_buffer(char *buffer, size_t buffer_size, const char *ptr, size_t size)
{
	size_t i;
	char *aux_buffer = buffer;
	size_t aux_buffer_size = buffer_size;
	int ret;

	for (i = 0; i < size; ++i) {
		if (ptr[i] == '"') {
			ret = snprintf(aux_buffer, aux_buffer_size, "\\\"");
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
		else if (isprint(ptr[i])) {
			if (aux_buffer_size > 0) {
				memcpy(aux_buffer, ptr + i, 1);
				aux_buffer = aux_buffer + 1;
				aux_buffer_size = aux_buffer_size - 1;
			}
		}
		else {
			ret = snprintf(aux_buffer, aux_buffer_size, "\\x%02x", (unsigned char)ptr[i]);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
	}

	return buffer_size - aux_buffer_size;
}

// Print the msgpack object. Debug only.
int msgpack_object_print_buffer(char *buffer, size_t buffer_size, const msgpack_object& o)
{
	char *aux_buffer = buffer;
	size_t aux_buffer_size = buffer_size;
	int ret;
	switch (o.type) {
	case MSGPACK_OBJECT_NIL:
		ret = snprintf(aux_buffer, aux_buffer_size, "nil");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_BOOLEAN:
		ret = snprintf(aux_buffer, aux_buffer_size, (o.via.boolean ? "true" : "false"));
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_POSITIVE_INTEGER:
#if defined(PRIu64)
		ret = snprintf(aux_buffer, aux_buffer_size, "%" PRIu64, o.via.u64);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
#else
		if (o.via.u64 > ULONG_MAX) {
			ret = snprintf(aux_buffer, aux_buffer_size, "over 4294967295");
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
		else {
			ret = snprintf(aux_buffer, aux_buffer_size, "%lu", (unsigned long)o.via.u64);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
#endif
		break;

	case MSGPACK_OBJECT_NEGATIVE_INTEGER:
#if defined(PRIi64)
		ret = snprintf(aux_buffer, aux_buffer_size, "%" PRIi64, o.via.i64);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
#else
		if (o.via.i64 > LONG_MAX) {
			ret = snprintf(aux_buffer, aux_buffer_size, "over +2147483647");
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
		else if (o.via.i64 < LONG_MIN) {
			ret = snprintf(aux_buffer, aux_buffer_size, "under -2147483648");
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
		else {
			ret = snprintf(aux_buffer, aux_buffer_size, "%ld", (signed long)o.via.i64);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
#endif
		break;

	case MSGPACK_OBJECT_FLOAT32:
	case MSGPACK_OBJECT_FLOAT64:
		ret = snprintf(aux_buffer, aux_buffer_size, "%f", o.via.f64);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_STR:
		ret = snprintf(aux_buffer, aux_buffer_size, "\"");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		ret = snprintf(aux_buffer, aux_buffer_size, "%.*s", o.via.str.size, o.via.str.ptr);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		ret = snprintf(aux_buffer, aux_buffer_size, "\"");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_BIN:
		ret = snprintf(aux_buffer, aux_buffer_size, "\"");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;

		ret = msgpack_object_bin_print_buffer(aux_buffer, aux_buffer_size, o.via.bin.ptr, o.via.bin.size);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;

		ret = snprintf(aux_buffer, aux_buffer_size, "\"");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_EXT:
#if defined(PRIi8)
		ret = snprintf(aux_buffer, aux_buffer_size, "(ext: %" PRIi8 ")", o.via.ext.type);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
#else
		ret = snprintf(aux_buffer, aux_buffer_size, "(ext: %d)", (int)o.via.ext.type);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
#endif
		ret = snprintf(aux_buffer, aux_buffer_size, "\"");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;

		ret = msgpack_object_bin_print_buffer(aux_buffer, aux_buffer_size, o.via.ext.ptr, o.via.ext.size);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;

		ret = snprintf(aux_buffer, aux_buffer_size, "\"");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_ARRAY:
		ret = snprintf(aux_buffer, aux_buffer_size, "[");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		if (o.via.array.size != 0) {
			msgpack_object* p = o.via.array.ptr;
			msgpack_object* const pend = o.via.array.ptr + o.via.array.size;
			ret = msgpack_object_print_buffer(aux_buffer, aux_buffer_size, *p);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
			++p;
			for (; p < pend; ++p) {
				ret = snprintf(aux_buffer, aux_buffer_size, ", ");
				aux_buffer = aux_buffer + ret;
				aux_buffer_size = aux_buffer_size - ret;
				ret = msgpack_object_print_buffer(aux_buffer, aux_buffer_size, *p);
				aux_buffer = aux_buffer + ret;
				aux_buffer_size = aux_buffer_size - ret;
			}
		}
		ret = snprintf(aux_buffer, aux_buffer_size, "]");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	case MSGPACK_OBJECT_MAP:
		ret = snprintf(aux_buffer, aux_buffer_size, "{");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		if (o.via.map.size != 0) {
			msgpack_object_kv* p = o.via.map.ptr;
			msgpack_object_kv* const pend = o.via.map.ptr + o.via.map.size;
			ret = msgpack_object_print_buffer(aux_buffer, aux_buffer_size, p->key);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
			ret = snprintf(aux_buffer, aux_buffer_size, "=>");
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
			ret = msgpack_object_print_buffer(aux_buffer, aux_buffer_size, p->val);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
			++p;
			for (; p < pend; ++p) {
				ret = snprintf(aux_buffer, aux_buffer_size, ", ");
				aux_buffer = aux_buffer + ret;
				aux_buffer_size = aux_buffer_size - ret;
				ret = msgpack_object_print_buffer(aux_buffer, aux_buffer_size, p->key);
				aux_buffer = aux_buffer + ret;
				aux_buffer_size = aux_buffer_size - ret;
				ret = snprintf(aux_buffer, aux_buffer_size, "=>");
				aux_buffer = aux_buffer + ret;
				aux_buffer_size = aux_buffer_size - ret;
				ret = msgpack_object_print_buffer(aux_buffer, aux_buffer_size, p->val);
				aux_buffer = aux_buffer + ret;
				aux_buffer_size = aux_buffer_size - ret;
			}
		}
		ret = snprintf(aux_buffer, aux_buffer_size, "}");
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
		break;

	default:
		// FIXME
#if defined(PRIu64)
		ret = snprintf(aux_buffer, aux_buffer_size, "#<UNKNOWN %i %" PRIu64 ">", o.type, o.via.u64);
		aux_buffer = aux_buffer + ret;
		aux_buffer_size = aux_buffer_size - ret;
#else
		if (o.via.u64 > ULONG_MAX) {
			ret = snprintf(aux_buffer, aux_buffer_size, "#<UNKNOWN %i over 4294967295>", o.type);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
		else {
			ret = snprintf(aux_buffer, aux_buffer_size, "#<UNKNOWN %i %lu>", o.type, (unsigned long)o.via.u64);
			aux_buffer = aux_buffer + ret;
			aux_buffer_size = aux_buffer_size - ret;
		}
#endif
	}

	return buffer_size - aux_buffer_size;
}

#endif // WITH_MSGPACK_CPLUSPLUS