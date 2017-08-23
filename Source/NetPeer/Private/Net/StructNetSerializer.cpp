// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#include "NetPeerPrivatePCH.h"
#include "StructNetSerializer.h"
#include "UE4FundamentalTypeWrapper/UE4FundamentalTypeBase.h"



#include "TcpNetPeer.h"

template<typename NetSerializerHelper>
bool IterateThroughStructProperty(UScriptStruct* StructTypeInfo, void* StructDataPtr, NetSerializerHelper& SerializerHelper);

/*
 *Serialize UProperty.
 */
struct FNetSerializerHelper
{
	FNetSerializerHelper(FArchive& DataStream) : Stream(DataStream)
	{
	}

	FArchive& Stream;

	/**
	* Gets the value of the given property.
	*
	* @param Property The property to set.
	* @param Data A pointer to the memory holding the property's data.
	* @return true on success, false otherwise.
	* @see ClearPropertyValue
	*/
	template<typename UPropertyType, typename PropertyType>
	bool Serialize(UProperty* Property, void* Data)
	{		
		UPropertyType* TypedProperty = Cast<UPropertyType>(Property);
		if (TypedProperty == nullptr)
		{
			check(false);
			return false;
		}

		Stream << *(PropertyType*)Data;
		return true;
	}

	void StructBegin(UScriptStruct* StructTypeInfo)
	{
	}

	void StructEnd()
	{
	}

	void ArrayBegin(int32 ArraySize)
	{
	}

	void ArrayEnd()
	{
	}

	bool SerializeBin(void* data, uint32_t count)
	{
		return true;
	}

	bool SerializeBin(FScriptArrayHelper& Helper)
	{
		return true;
	}

	/**
	* Write the size of array.
	* @param Helper The helper of write array's data.
	* @return true on success, false otherwise.
	*/
	bool SerializeArraySize(FScriptArrayHelper& Helper)
	{
		if (Helper.Num() > MAX_uint16)
		{
			check(false);
			return false;
		}

		uint16 uNum = (uint16)Helper.Num();
		Stream << uNum;
		return true;
	}
};

/*
* Deserialize UProperty.
*/
struct FNetDeserializerHelper
{
	FNetDeserializerHelper(FArchive& DataStream) : Stream(DataStream)
	{
	}

	FArchive& Stream;

	void StructBegin(UScriptStruct* StructTypeInfo)
	{	
	}

	void StructEnd()
	{	
	}

	void ArrayBegin(int32 ArraySize)
	{		
	}

	void ArrayEnd()
	{
	}

	bool SerializeBin(void* data, uint32_t count)
	{	
		return true;
	}

	bool SerializeBin(FScriptArrayHelper& Helper)
	{
		return true;
	}

	/**
	* Sets the value of the given property.
	*
	* @param Property The property to set.
	* @param Data A pointer to the memory holding the property's data.
	* @return true on success, false otherwise.
	* @see ClearPropertyValue
	*/
	template<typename UPropertyType, typename PropertyType>
	bool Serialize(UProperty* Property, void* Data)
	{
		UPropertyType* TypedProperty = Cast<UPropertyType>(Property);
		if (TypedProperty == nullptr)
		{
			check(false);
			return false;
		}

		PropertyType Value;
		Stream << Value;
		*(PropertyType*)Data = Value;
		return true;
	}

	/**
	* Read the size of array.
	* @param Helper The helper of array's data read.
	* @return true on success, false otherwise.
	*/
	bool SerializeArraySize(FScriptArrayHelper& Helper)
	{
		if (Helper.Num() > MAX_uint16)
		{
			check(false);
			return false;
		}

		uint16 uNum = 0;
		Stream << uNum;
		Helper.Resize(uNum);
		return true;
	}
};

/* parsing a single property
* @param Property    the property reflection data
* @param ValuePtr    the pointer to the property value
* @return ture for Parse successed, false for Parse failed.
*/
template<typename NetSerializerHelper>
bool ParseProperty(UProperty* Property, void* ValuePtr, NetSerializerHelper& SerializerHelper)
{
	if (Cast<UNumericProperty>(Property) != nullptr)
	{
		if (Property->GetClass() == UByteProperty::StaticClass())
		{
			return SerializerHelper.template Serialize<UByteProperty, uint8>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UInt8Property::StaticClass())
		{
			return SerializerHelper.template Serialize<UInt8Property, int8>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UDoubleProperty::StaticClass())
		{
			return SerializerHelper.template Serialize<UDoubleProperty, double>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UFloatProperty::StaticClass())
		{
			return SerializerHelper.template Serialize<UFloatProperty, float>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UIntProperty::StaticClass())
		{
			return SerializerHelper.template Serialize<UIntProperty, int32>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UUInt32Property::StaticClass())
		{
			return SerializerHelper.template Serialize<UUInt32Property, uint32>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UInt16Property::StaticClass())
		{
			return SerializerHelper.template Serialize<UInt16Property, int16>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UUInt16Property::StaticClass())
		{
			return SerializerHelper.template Serialize<UUInt16Property, uint16>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UInt64Property::StaticClass())
		{
			return SerializerHelper.template Serialize<UInt64Property, int64>(Property, ValuePtr);
		}
		else if (Property->GetClass() == UUInt64Property::StaticClass())
		{
			return SerializerHelper.template Serialize<UUInt64Property, uint64>(Property, ValuePtr);
		}		
	}
	else if (Property->GetClass() == UBoolProperty::StaticClass())
	{
		return SerializerHelper.template Serialize<UBoolProperty, bool>(Property, ValuePtr);
	}
	else if (Property->GetClass() == UStrProperty::StaticClass())
	{
		return SerializerHelper.template Serialize<UStrProperty, FString>(Property, ValuePtr);
	}
	else if (Property->GetClass() == UNameProperty::StaticClass())
	{
		return SerializerHelper.template Serialize<UNameProperty, FName>(Property, ValuePtr);
	}
	else if (Property->GetClass() == UTextProperty::StaticClass())
	{
		return SerializerHelper.template Serialize<UTextProperty, FText>(Property, ValuePtr);
	}
	else if (UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Property))
	{
		// We need the helper to get to the items of the array            
		FScriptArrayHelper Helper(ArrayProperty, ValuePtr);

		if (ArrayProperty->Inner->GetClass() == UByteProperty::StaticClass())
		{	// see msgpack/v1/adaptor/vector_unsigned_char.hpp
			return SerializerHelper.SerializeBin(Helper);
		}
		else
		{
			if (!SerializerHelper.SerializeArraySize(Helper))
				return false;
			for (int32 i = 0, n = Helper.Num(); i < n; ++i)
			{
				if (!ParseProperty(ArrayProperty->Inner, Helper.GetRawPtr(i), SerializerHelper))
				{
					UE_LOG(LogNetSerialization, Error, TEXT("De/Serialize Property %s (%s) failed."), *ArrayProperty->Inner->GetName(), *ArrayProperty->Inner->GetClass()->GetName());
					return false;
				}
			}
			SerializerHelper.ArrayEnd();
		}
		return true;
	}
	else if (UStructProperty* StructProperty = Cast<UStructProperty>(Property))
	{	
		if (StructProperty->Struct->IsChildOf(FRawDataTypeBase::StaticStruct()))
		{
			TFieldIterator<UProperty> It(StructProperty->Struct);
			UProperty* RawDataProperty = *It;
			// This grabs the pointer to where the property's value is stored.
			void* PropertyValuePtr = RawDataProperty->ContainerPtrToValuePtr<void>(ValuePtr, 0);
			if (++It, It)
				check(false);	// Only one property is allowed.
			return ParseProperty(RawDataProperty, PropertyValuePtr, SerializerHelper);
			
		}
		else	// Reading a nested struct
			return IterateThroughStructProperty(StructProperty->Struct, ValuePtr, SerializerHelper);
	}
	
	check(false);
	UE_LOG(LogNetSerialization, Error, TEXT("Property %s (%s) is not supported"), *Property->GetFName().ToString(), *Property->GetClass()->GetName());

	return false;
}

/*
 * Iterating through all properties of a struct
 * @param StructProperty	The struct property reflection data.
 * @param StructDataPtr		The pointer to the struct value.
 */
template<typename NetSerializerHelper>
bool IterateThroughStructProperty(UScriptStruct* StructTypeInfo, void* StructDataPtr, NetSerializerHelper& SerializerHelper)
{
	SerializerHelper.StructBegin(StructTypeInfo);
	
	for (TFieldIterator<UProperty> It(StructTypeInfo); It; ++It)
	{
		UProperty* Property = *It;

		FString VariableName = Property->GetName();

		if (Property->ArrayDim > 1)
		{	// Static array?
			if (Property->GetClass() == UByteProperty::StaticClass())
			{	// See msgpack/v1/adaptor/carray.hpp (struct convert<unsigned char[N]>)
				uint32 size = Property->ArrayDim;				
				void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructDataPtr, 0);
				if (!SerializerHelper.SerializeBin(ValuePtr, size))
				{
					UE_LOG(LogNetSerialization, Error, TEXT("De/Serialize Property %s (%s) failed."), *Property->GetName(), *Property->GetClass()->GetName());
					return false;
				}
				continue;
			}
			else
				SerializerHelper.ArrayBegin(Property->ArrayDim);
		}
		
		// Never assume ArrayDim is always 1
		for (int32 ArrayIndex = 0; ArrayIndex < Property->ArrayDim; ArrayIndex++)
		{
			// This grabs the pointer to where the property's value is stored.
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructDataPtr, ArrayIndex);

			// Parse this property.
			if (!ParseProperty(Property, ValuePtr, SerializerHelper))
			{
				UE_LOG(LogNetSerialization, Error, TEXT("De/Serialize Property %s (%s) failed."), *Property->GetName(), *Property->GetClass()->GetName());
				return false;
			}
		}

		if (Property->ArrayDim > 1)
		{
			SerializerHelper.ArrayEnd();
		}
	}

	SerializerHelper.StructEnd();
	return true;
}

bool FStructNetSerializer::Serialize(const void* StructDataPtr, UScriptStruct& StructTypeInfo, FArchive& Steam)
{
	FNetSerializerHelper SerializerHelper(Steam);
	return IterateThroughStructProperty(&StructTypeInfo, (void*)StructDataPtr, SerializerHelper);
}

bool FStructNetSerializer::Deserialize(void* StructDataPtr, UScriptStruct& StructTypeInfo, FArchive& Steam)
{
	FNetDeserializerHelper DeserializerHelper(Steam);
	return IterateThroughStructProperty(&StructTypeInfo, (void*)StructDataPtr, DeserializerHelper);
}

void mpack_write(mpack_writer_t* writer, const FString& Data)
{
	mpack_write_utf8_cstr(writer, TCHAR_TO_UTF8(*Data));
}
void mpack_write(mpack_writer_t* writer, const FName& Data)
{
	mpack_write_utf8_cstr(writer, TCHAR_TO_UTF8(*Data.ToString()));
}
void mpack_write(mpack_writer_t* writer, const FText& Data)
{
	mpack_write_utf8_cstr(writer, TCHAR_TO_UTF8(*Data.ToString()));
}

/*
* Serialize UProperty with mpack.
*/
struct FMPackSerializerHelper
{
	FMPackSerializerHelper(mpack_writer_t& DataStream) : Stream(DataStream)
	{
	}

	mpack_writer_t& Stream;

	/**
	* Gets the value of the given property.
	*
	* @param Property The property to set.
	* @param Data A pointer to the memory holding the property's data.
	* @return true on success, false otherwise.
	* @see ClearPropertyValue
	*/
	template<typename UPropertyType, typename PropertyType>
	bool Serialize(UProperty* Property, void* Data)
	{
		UPropertyType* TypedProperty = Cast<UPropertyType>(Property);
		if (TypedProperty == nullptr)
		{
			check(false);
			return false;
		}

		mpack_write(&Stream, *(PropertyType*)Data);
		return true;
	}

	void StructBegin(UScriptStruct* StructTypeInfo)
	{
		int32 nPropertyCount = 0;
		// Calc the count of properties.
		for (TFieldIterator<UProperty> It(StructTypeInfo); It; ++It, nPropertyCount++);
		mpack_start_array(&Stream, nPropertyCount);
	}

	void StructEnd()
	{
		mpack_finish_array(&Stream);
	}

	void ArrayBegin(uint32 ArraySize)
	{
		mpack_start_array(&Stream, ArraySize);
	}

	void ArrayEnd()
	{
		mpack_finish_array(&Stream);
	}

	bool SerializeBin(void* data, uint32_t count)
	{
		mpack_start_bin(&Stream, count);
		if(count > 0)
			mpack_write_bytes(&Stream, (const char*)data, count);
		mpack_finish_bin(&Stream);

		return true;
	}

	bool SerializeBin(FScriptArrayHelper& Helper)
	{
		int32 count = Helper.Num();
		mpack_start_bin(&Stream, count);
		if (count > 0)
		{
			mpack_write_bytes(&Stream, (const char*)Helper.GetRawPtr(0), count);
		}
		mpack_finish_bin(&Stream);
		return true;
	}

	/**
	* Write the count of array.
	* @param Helper 
	* @return true on success, false otherwise.
	*/
	bool SerializeArraySize(FScriptArrayHelper& Helper)
	{
		if (Helper.Num() > MAX_uint16)
		{
			check(false);
			return false;
		}

		mpack_start_array(&Stream, Helper.Num());
		return true;
	}

	/**
	 * End write the array's data.
	 **/
	void FinishArray()
	{
		mpack_finish_array(&Stream);
	}
};

TCHAR* MPackErrorToTChar(mpack_error_t error)
{
	return ANSI_TO_TCHAR(mpack_error_to_string(error));
}

/*
* Deserialize UProperty with mpack.
*/
struct FMPackDeserializerHelper
{
	FMPackDeserializerHelper(mpack_reader_t& DataStream) : Stream(DataStream)
	{
	}

	mpack_reader_t& Stream;

	/**
	 * If is in error state return true.
	 **/
	bool IsInError() const
	{
		return mpack_reader_error(&Stream) == mpack_ok;
	}
	
	/**
	* Gets the value of the given property.
	*
	* @param Property The property to set.
	* @param Data A pointer to the memory holding the property's data.
	* @return true on success, false otherwise.
	* @see ClearPropertyValue
	*/
	template<typename UPropertyType, typename PropertyType>
	bool Serialize(UProperty* Property, void* Data)
	{
		UPropertyType* TypedProperty = Cast<UPropertyType>(Property);
		if (TypedProperty == nullptr)
		{
			check(false);
			return false;
		}

#if !UE_BUILD_SHIPPING
		// For debug.
		const char* data = Stream.data;
		size_t left = Stream.left;
#endif
		mpack_read(*(PropertyType*)Data);

		mpack_error_t error = mpack_reader_error(&Stream);
		if (error != mpack_ok)
		{
#if !UE_BUILD_SHIPPING
			if (Stream.error == mpack_error_type)
			{
				Stream.error = mpack_ok;
				Stream.data = data;
				Stream.left = left;
				mpack_tag_t var = mpack_read_tag(&Stream);
				Stream.error = mpack_error_type;
				UE_LOG(LogNetSerialization, Error, TEXT("Read struct property %s (%s) failed(Err=%s), the data type of propery is '%s')"), *TypedProperty->GetFName().ToString(), *TypedProperty->GetClass()->GetName(), MPackErrorToTChar(error), ANSI_TO_TCHAR(mpack_type_to_string(var.type)));
			}			
			else
			{
				UE_LOG(LogNetSerialization, Error, TEXT("Read struct property %s (%s) failed(Err=%s)"), *TypedProperty->GetFName().ToString(), *TypedProperty->GetClass()->GetName(), MPackErrorToTChar(error));
			}
#else
			UE_LOG(LogNetSerialization, Error, TEXT("Read struct property %s (%s) failed(Err=%s)"), *TypedProperty->GetFName().ToString(), *TypedProperty->GetClass()->GetName(), MPackErrorToTChar(error));
#endif	
			return false;
		}
		return true;
	}

	void mpack_read(int8& value) {
		value = mpack_expect_i8(&Stream);
	}
	void mpack_read(uint8& value) {
		value = mpack_expect_u8(&Stream);
	}
	void mpack_read(int16& value) {
		value = mpack_expect_i16(&Stream);
	}
	void mpack_read(uint16& value) {
		value = mpack_expect_u16(&Stream);
	}
	void mpack_read(int32& value) {
		value = mpack_expect_i32(&Stream);
	}
	void mpack_read(uint32& value) {
		value = mpack_expect_u32(&Stream);
	}
	void mpack_read(int64& value) {
		value = mpack_expect_i64(&Stream);
	}
	void mpack_read(uint64& value) {
		value = mpack_expect_u64(&Stream);
	}
	void mpack_read(float& value) {
		value = mpack_expect_float_strict(&Stream);
	}
	void mpack_read(double& value) {
		value = mpack_expect_double_strict(&Stream);
	}
	void mpack_read(bool& value) {
		value = mpack_expect_bool(&Stream);
	}
	void mpack_read(FString& Data)
	{
		uint32_t length = mpack_expect_str(&Stream);
		if (mpack_reader_error(&Stream) != mpack_ok)
			return;
		const char* str = mpack_read_utf8_inplace(&Stream, length);
		if (str != NULL)
		{
			FUTF8ToTCHAR _tchar(str, length);
			Data = FString(_tchar.Length(), _tchar.Get());
		}
	}
	void mpack_read(FName& Data)
	{
		uint32_t length = mpack_expect_str(&Stream);
		if (mpack_reader_error(&Stream) != mpack_ok)
			return;
		const char* str = mpack_read_utf8_inplace(&Stream, length);
		if (str != NULL)
		{
			FUTF8ToTCHAR _tchar(str, length);
			Data = *FString(_tchar.Length(), _tchar.Get());
		}
	}
	void mpack_read(FText& Data)
	{
		uint32_t length = mpack_expect_str(&Stream);
		if (mpack_reader_error(&Stream) != mpack_ok)
			return;
		const char* str = mpack_read_utf8_inplace(&Stream, length);
		if (str != NULL)
		{
			FUTF8ToTCHAR _tchar(str, length);			
			Data.FromString(FString(_tchar.Length(), _tchar.Get()));
		}
	}

	void StructBegin(UScriptStruct* StructTypeInfo)
	{
		int32 nPropertyCount = 0;
		// Calc the count of properties.
		for (TFieldIterator<UProperty> It(StructTypeInfo); It; ++It, nPropertyCount++);
		mpack_expect_array_match(&Stream, nPropertyCount);
	}

	void StructEnd()
	{
		mpack_done_array(&Stream);
	}

	void ArrayBegin(uint32 ArraySize)
	{
		mpack_expect_array_match(&Stream, ArraySize);
	}

	void ArrayEnd()
	{
		mpack_done_array(&Stream);
	}

	bool SerializeBin(void* data, uint32_t count)
	{
		mpack_expect_bin_buf(&Stream, (char*)data, count);
		return IsInError();
	}

	bool SerializeBin(FScriptArrayHelper& Helper)
	{		
		uint32 count = mpack_expect_bin_max(&Stream, 9999);
		Helper.Resize(count);
		if (count > 0)
		{
			mpack_read_bytes(&Stream, (char*)Helper.GetRawPtr(0), count);
		}
		mpack_done_bin(&Stream);
		return IsInError();
	}

	/**
	* Read the count of array.
	* @param Helper
	* @return true on success, false otherwise.
	*/
	bool SerializeArraySize(FScriptArrayHelper& Helper)
	{
		int32 count = mpack_expect_array_max(&Stream, 10000);
		if (IsInError())
		{
			Helper.Resize(count);
			return true;
		}
		return false;
	}

	/**
	* End read the array's data.
	**/
	void FinishArray()
	{
		mpack_done_array(&Stream);
	}
};

static void my_growable_writer_flush(mpack_writer_t* writer, const char* data, size_t count) {
	FMessagePacket* pMessagePacket = (FMessagePacket*)writer->context;
	pMessagePacket->write(data, count);
}

void mpack_error_handle(mpack_writer_t* writer, mpack_error_t error)
{
#if !UE_BUILD_SHIPPING
	// For easy debug.
	UE_LOG(LogNetSerialization, Error, TEXT("Serialize error (Err=%s)"), MPackErrorToTChar(error));
#endif
}

void mpack_error_handle(mpack_reader_t* writer, mpack_error_t error)
{
#if !UE_BUILD_SHIPPING
	// For easy debug.
	UE_LOG(LogNetSerialization, Error, TEXT("Deserialize error (Err=%s)"), MPackErrorToTChar(error));
#endif
}

bool FStructNetSerializer::MSerialize(const void* StructDataPtr, UScriptStruct& StructTypeInfo, FMessagePacket& Steam)
{	
	mpack_error_t ret;

	const int nBufSize = 1024;
	char buf[nBufSize];
	mpack_writer_t writer;
	mpack_writer_init(&writer, buf, nBufSize);
	mpack_writer_set_context(&writer, &Steam);
	mpack_writer_set_error_handler(&writer, mpack_error_handle);
	mpack_writer_set_flush(&writer, my_growable_writer_flush);
	
	FMPackSerializerHelper SerializerHelper(writer);
	if (!IterateThroughStructProperty(&StructTypeInfo, (void*)StructDataPtr, SerializerHelper))
	{
		mpack_writer_destroy(&writer);
		return false;
	}

	ret = mpack_writer_destroy(&writer);
	if (ret != mpack_ok)
	{
		UE_LOG(LogNetSerialization, Error, TEXT("Serialize message struct %s (%s) failed(Err=%s)"), *StructTypeInfo.GetFName().ToString(), *StructTypeInfo.GetClass()->GetName(), MPackErrorToTChar(ret));
		return false;
	}

	return true;
}

bool FStructNetSerializer::MDeserialize(void* StructDataPtr, UScriptStruct& StructTypeInfo, const uint8* Data, uint32 len)
{
	mpack_reader_t reader;
	mpack_reader_init_data(&reader, (const char*)Data, len);
	mpack_reader_set_error_handler(&reader, mpack_error_handle);		// For easy debug.

	FMPackDeserializerHelper DeserializerHelper(reader);
	if (!IterateThroughStructProperty(&StructTypeInfo, (void*)StructDataPtr, DeserializerHelper))
	{
		mpack_reader_destroy(&reader);
		return false;
	}
		
	mpack_error_t error = mpack_reader_destroy(&reader);
	if (error != mpack_ok)
	{
		UE_LOG(LogNetSerialization, Error, TEXT("Serialize message struct %s (%s) failed(Err=%s)"), *StructTypeInfo.GetFName().ToString(), *StructTypeInfo.GetClass()->GetName(), MPackErrorToTChar(error));
		return false;
	}
	return true;
}
