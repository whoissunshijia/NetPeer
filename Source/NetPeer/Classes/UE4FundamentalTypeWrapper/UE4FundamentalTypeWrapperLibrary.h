// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once

#include "Double.h"
#include "Int8.h"
#include "Int16.h"
#include "UInt16.h"
#include "UInt32.h"
#include "Int64.h"
#include "UInt64.h"
#include "UE4FundamentalTypeWrapperLibrary.generated.h"


UCLASS( ClassGroup = "NetPeer|CPP UE4FundamentalType Wrapper" )
class UUE4FundamentalTypeWrapper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/************/
	/* Creators */
	/************/

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "Create Double", meta = (BlueprintAutocast))
	static FADouble CreateDouble( float Value )
	{
		return { static_cast< double >( Value ) };
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "Create Int 64" , meta = (BlueprintAutocast) )
	static FAInt64 CreateInt64( int32 Value )
	{
		return { static_cast< int64 >( Value ) };
	}

	// The low value is the first 31 bits. The high mid value is bits 32 to 62. The high value is the last bit.
	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "Create Int 64 Full" )
	static FAInt64 CreateInt64Full( bool bNegative, int32 Low, int32 Mid, uint8 High )
	{
		if ( bNegative )
			return -static_cast< int64 >( Low > 0 ? Low : 0 ) -
			( static_cast< int64 >( Mid > 0 ? Mid : 0 ) << 31 ) -
			( static_cast< int64 >( ( High > 0 ? High : 0 ) & 1 ) << 62 );

		else
			return static_cast< int64 >( Low > 0 ? Low : 0 ) +
			( static_cast< int64 >( Mid > 0 ? Mid : 0 ) << 31 ) +
			( static_cast< int64 >( ( High > 0 ? High : 0 ) & 1 ) << 62 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "Create UInt 64" )
	static FAUInt64 CreateUInt64( int32 Value )
	{
		return { static_cast< uint64 >( Value ) };
	}

	// The low value is the first 31 bits. The high mid value is bits 32 to 62. The high value is the last 2 bits.
	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "Create UInt 64 Full" )
	static FAInt64 CreateUInt64Full( int32 Low, int32 Mid, uint8 High )
	{
		return static_cast< uint64 >( Low > 0 ? Low : 0 ) +
			( static_cast< uint64 >( Mid > 0 ? Mid : 0 ) << 31 ) +
			( static_cast< uint64 >( ( High > 0 ? High : 0 ) & 3 ) << 62 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "Create UInt 32" )
	static FAUInt32 CreateUInt32( int32 Value )
	{
		return { static_cast< uint16 >( Value ) };
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "Create Int 16" )
	static FAInt16 CreateInt16( int32 Value )
	{
		return { static_cast< int16 >( Value ) };
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "Create UInt 16" )
	static FAUInt16 CreateUInt16( int32 Value )
	{
		return { static_cast< uint16 >( Value ) };
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "Create Int 8" )
	static FAInt8 CreateInt8( int32 Value )
	{
		return { static_cast< int8 >( Value ) };
	}


	/*************/
	/* Extractors */
	/*************/

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "double #" )
	static void ExtractDouble( const FADouble& D, float& F )
	{
		F = static_cast< float >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 #" )
	static void ExtractInt64( const FAInt64& I64, int32& I32 )
	{
		I32 = static_cast< int32 >( I64.Value );
	}

	// Low is the first 31 bits. Mid is bits 32 to 62. High is the last bit.
	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 #F" )
	static void ExtractInt64Full( const FAInt64& I64, bool& bNegative, int32& Low, int32& Mid, uint8& High )
	{
		bNegative = I64.Value < 0;
		Low = ( bNegative ? -I64.Value : I64.Value ) & 0x7FFFFFFF;
		Mid = ( ( bNegative ? -I64.Value : I64.Value ) >> 31 ) & 0x7FFFFFFF;
		High = ( ( bNegative ? -I64.Value : I64.Value ) >> 62 ) & 0x1;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 #" )
	static void ExtractUInt64( const FAUInt64& UI64, int32& I32 )
	{
		I32 = static_cast< int32 >( UI64.Value & 0x7FFFFFFF );
	}

	// Low is the first 31 bits. Mid is bits 32 to 62. High is the last two bits.
	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 #F" )
	static void ExtractUInt64Full( const FAUInt64& UI64, int32& Low, int32& Mid, uint8& High )
	{
		Low = UI64.Value & 0x7FFFFFFF;
		Mid = ( UI64.Value >> 31 ) & 0x7FFFFFFF;
		High = ( UI64.Value >> 62 ) & 0x3;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 #" )
	static void ExtractUInt32( const FAUInt32& UI32, int32& I32 )
	{
		I32 = static_cast< int32 >( UI32.Value & 0x7FFFFFFF );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 #" )
	static void ExtractUInt32Full( const FAUInt32& UI32, int32& I32, uint8& HighBit )
	{
		I32 = static_cast< int32 >( UI32.Value & 0x7FFFFFFF );
		HighBit = UI32.Value >> 31;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 #" )
	static void ExtractInt16( const FAInt16& I16, int32& I32 )
	{
		I32 = static_cast< int32 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 #" )
	static void ExtractUInt16( const FAUInt16& UI16, int32& I32 )
	{
		I32 = static_cast< int32 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 #" )
	static void ExtractInt8( const FAInt8& I8, int32& I32 )
	{
		I32 = static_cast< int32 >( I8.Value );
	}


	/*********/
	/* Casts */
	/*********/

	/* To String */

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (int8)")
	static FString CastInt8ToString(const FAInt8& D)
	{
		return TTypeToString<int8>::ToString(D.Value);
	}

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (Double)")
	static FString CastDoubleToString(const FADouble& D)
	{
		return TTypeToString<double>::ToString(D.Value);
	}

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (int16)")
		static FString CastInt16ToString(const FAInt16& D)
	{
		return TTypeToString<int16>::ToString(D.Value);
	}

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (int64)")
		static FString CastInt64ToString(const FAInt64& D)
	{
		return TTypeToString<int64>::ToString(D.Value);
	}

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (uint16)")
		static FString CastUInt16ToString(const FAUInt16& D)
	{
		return TTypeToString<uint16>::ToString(D.Value);
	}

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (uint32)")
		static FString CastUInt32ToString(const FAUInt32& D)
	{
		return TTypeToString<uint32>::ToString(D.Value);
	}

	UFUNCTION(Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "ToString (uint64)")
		static FString CastUInt64ToString(const FAUInt64& D)
	{
		return TTypeToString<uint64>::ToString(D.Value);
	}

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> flt" )
	static float CastDoubleToFloat( const FADouble& D )
	{
		return static_cast< float >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> dbl" )
	static FADouble CastFloatToDouble( float F )
	{
		return static_cast< double >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> int64" )
	static FAInt64 CastDoubleToInt64( const FADouble& D )
	{
		return static_cast< int64 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> dbl" )
	static FADouble CastInt64ToDouble( const FAInt64& I64 )
	{
		return static_cast< double >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> uint64" )
	static FAUInt64 CastDoubleToUInt64( const FADouble& D )
	{
		return static_cast< uint64 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> dbl" )
	static FADouble CastUInt64ToDouble( const FAUInt64& UI64 )
	{
		return static_cast< double >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> int32" )
	static int32 CastDoubleToInt32( const FADouble& D )
	{
		return static_cast< int32 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> dbl" )
	static FADouble CastInt32ToDouble( int32 I32 )
	{
		return static_cast< double >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> uint32" )
	static FAUInt32 CastDoubleToUInt32( const FADouble& D )
	{
		return static_cast< uint32 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> dbl" )
	static FADouble CastUInt32ToDouble( const FAUInt32& UI32 )
	{
		return static_cast< double >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> int16" )
	static FAInt16 CastDoubleToInt16( const FADouble& D )
	{
		return static_cast< int16 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int16 -> dbl" )
	static FADouble CastInt16ToDouble( const FAInt16& I16 )
	{
		return static_cast< double >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> uint16" )
	static FAUInt16 CastDoubleToUInt16( const FADouble& D )
	{
		return static_cast< uint16 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint16 -> dbl" )
	static FADouble CastUInt16ToDouble( const FAUInt16& UI16 )
	{
		return static_cast< double >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> int8" )
	static FAInt8 CastDoubleToInt8( const FADouble& D )
	{
		return static_cast< int8 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int8 -> dbl" )
	static FADouble CastInt8ToDouble( const FAInt8& I8 )
	{
		return static_cast< double >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "dbl -> uint8" )
	static uint8 CastDoubleToUInt8( const FADouble& D )
	{
		return static_cast< uint8 >( D.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint8 -> dbl" )
	static FADouble CastUInt8ToDouble( uint8 UI8 )
	{
		return static_cast< double >( UI8 );
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> int64" )
	static FAInt64 CastFloatToInt64( float F )
	{
		return static_cast< int64 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> flt" )
	static float CastInt64ToFloat( const FAInt64& I64 )
	{
		return static_cast< float >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> uint64" )
	static FAUInt64 CastFloatToUInt64( float F )
	{
		return static_cast< uint64 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> flt" )
	static float CastUInt64ToFloat( const FAUInt64& UI64 )
	{
		return static_cast< float >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> int32" )
	static int32 CastFloatToInt32( float F )
	{
		return static_cast< int32 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> flt" )
	static float CastInt32ToFloat( int32 I32 )
	{
		return static_cast< float >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> uint32" )
	static FAUInt32 CastFloatToUInt32( float F )
	{
		return static_cast< uint32 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> flt" )
	static float CastUInt32ToFloat( const FAUInt32& UI32 )
	{
		return static_cast< float >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> int16" )
	static FAInt16 CastFloatToInt16( float F )
	{
		return static_cast< int16 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int16 -> flt" )
	static float CastInt16ToFloat( const FAInt16& I16 )
	{
		return static_cast< float >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> uint16" )
	static FAUInt16 CastFloatToUInt16( float F )
	{
		return static_cast< uint16 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint16 -> flt" )
	static float CastUInt16ToFloat( const FAUInt16& UI16 )
	{
		return static_cast< float >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> int8" )
	static FAInt8 CastFloatToInt8( float F )
	{
		return static_cast< int8 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int8 -> flt" )
	static float CastInt8ToFloat( const FAInt8& I8 )
	{
		return static_cast< float >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "flt -> uint8" )
	static uint8 CastFloatToUInt8( float F )
	{
		return static_cast< uint8 >( F );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint8 -> flt" )
	static float CastUInt8ToFloat( uint8 UI8 )
	{
		return static_cast< float >( UI8 );
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> uint64" )
	static FAUInt64 CastInt64ToUInt64( const FAInt64& I64 )
	{
		return static_cast< uint64 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> int64" )
	static FAInt64 CastUInt64ToInt64( const FAUInt64& UI64 )
	{
		return static_cast< int64 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> int32" )
	static int32 CastInt64ToInt32( const FAInt64& I64 )
	{
		return static_cast< int32 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> int64" )
	static FAInt64 CastInt32ToInt64( int32 I32 )
	{
		return static_cast< int64 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> uint32" )
	static FAUInt32 CastInt64ToUInt32( const FAInt64& I64 )
	{
		return static_cast< uint32 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> int64" )
	static FAInt64 CastUInt32ToInt64( const FAUInt32& UI32 )
	{
		return static_cast< int64 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> int16" )
	static FAInt16 CastInt64ToInt16( const FAInt64& I64 )
	{
		return static_cast< int16 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int16 -> int64" )
	static FAInt64 CastInt16ToInt64( const FAInt16& I16 )
	{
		return static_cast< int64 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> uint16" )
	static FAUInt16 CastInt64ToUInt16( const FAInt64& I64 )
	{
		return static_cast< uint16 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint16 -> int64" )
	static FAInt64 CastUInt16ToInt64( const FAUInt16& UI16 )
	{
		return static_cast< int64 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> int8" )
	static FAInt8 CastInt64ToInt8( const FAInt64& I64 )
	{
		return static_cast< int8 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int8 -> int64" )
	static FAInt64 CastInt8ToInt64( const FAInt8& I8 )
	{
		return static_cast< int64 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int64 -> uint8" )
	static uint8 CastInt64ToUInt8( const FAInt64& I64 )
	{
		return static_cast< uint8 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint8 -> int64" )
	static FAInt64 CastUInt8ToInt64( uint8 UI8 )
	{
		return static_cast< int64 >( UI8 );
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> int32" )
	static int32 CastUInt64ToInt32( const FAUInt64& UI64 )
	{
		return static_cast< int32 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> uint64" )
	static FAUInt64 CastInt32ToUInt64( int32 I32 )
	{
		return static_cast< uint64 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> uint32" )
	static FAUInt32 CastUInt64ToUInt32( const FAUInt64& UI64 )
	{
		return static_cast< uint32 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> uint64" )
	static FAUInt64 CastUInt32ToUInt64( const FAUInt32& UI32 )
	{
		return static_cast< uint64 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> int16" )
	static FAInt16 CastUInt64ToInt16( const FAUInt64& UI64 )
	{
		return static_cast< int16 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int16 -> uint64" )
	static FAUInt64 CastInt16ToUInt64( const FAInt16& I16 )
	{
		return static_cast< uint64 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> uint16" )
	static FAUInt16 CastUInt64ToUInt16( const FAUInt64& UI64 )
	{
		return static_cast< uint16 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint16 -> uint64" )
	static FAUInt64 CastUInt16ToUInt64( const FAUInt16& UI16 )
	{
		return static_cast< uint64 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> int8" )
	static FAInt8 CastUInt64ToInt8( const FAUInt64& UI64 )
	{
		return static_cast< int8 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int8 -> uint64" )
	static FAUInt64 CastInt8ToUInt64( const FAInt8& I8 )
	{
		return static_cast< uint64 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint64 -> uint8" )
	static uint8 CastUInt64ToUInt8( const FAUInt64& UI64 )
	{
		return static_cast< uint8 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint8 -> uint64" )
	static FAUInt64 CastUInt8ToUInt64( uint8 UI8 )
	{
		return static_cast< uint64 >( UI8 );
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> uint32" )
	static FAUInt32 CastInt32ToUInt32( int32 I32 )
	{
		return static_cast< uint32 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> int32" )
	static int32 CastUInt32ToInt32( const FAUInt32& UI32 )
	{
		return static_cast< int32 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> int16" )
	static FAInt16 CastInt32ToInt16( int32 I32 )
	{
		return static_cast< int16 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int16 -> int32" )
	static int32 CastInt16ToInt32( const FAInt16& I16 )
	{
		return static_cast< int32 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> uint16" )
	static FAUInt16 CastInt32ToUInt16( int32 I32 )
	{
		return static_cast< uint16 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint16 -> int32" )
	static int32 CastUInt16ToInt32( const FAUInt16& UI16 )
	{
		return static_cast< int32 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> int8" )
	static FAInt8 CastInt32ToInt8( int32 I32 )
	{
		return static_cast< int8 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int8 -> int32" )
	static int32 CastInt8ToInt32( const FAInt8& I8 )
	{
		return static_cast< int32 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int32 -> uint8" )
	static uint8 CastInt32ToUInt8( int32 I32 )
	{
		return static_cast< uint8 >( I32 );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint8 -> int32" )
	static int32 CastUInt8ToInt32( uint8 UI8 )
	{
		return static_cast< int32 >( UI8 );
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> int16" )
	static FAInt16 CastUInt32ToInt16( const FAUInt32& UI32 )
	{
		return static_cast< int16 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "int16 -> uint32" )
	static FAUInt32 CastInt16ToUInt32( const FAInt16& I16 )
	{
		return static_cast< uint32 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast), DisplayName = "uint32 -> uint16" )
	static FAUInt16 CastUInt32ToUInt16( const FAUInt32& UI32 )
	{
		return static_cast< uint16 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint16 -> uint32" )
	static FAUInt32 CastUInt16ToUInt32( const FAUInt16& UI16 )
	{
		return static_cast< uint32 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint32 -> int8" )
	static FAInt8 CastUInt32ToInt8( const FAUInt32& UI32 )
	{
		return static_cast< int8 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int8 -> uint32" )
	static FAUInt32 CastInt8ToUInt32( const FAInt8& I8 )
	{
		return static_cast< uint32 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint32 -> uint8" )
	static uint8 CastUInt32ToUInt8( const FAUInt32& UI32 )
	{
		return static_cast< uint8 >( UI32.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint8 -> uint32" )
	static FAUInt32 CastUInt8ToUInt32( uint8 UI8 )
	{
		return static_cast< uint32 >( UI8 );
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int16 -> uint16" )
	static FAUInt16 CastInt16ToUInt16( const FAInt16& I16 )
	{
		return static_cast< uint16 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint16 -> int16" )
	static FAInt16 CastUInt16ToInt16( const FAUInt16& UI16 )
	{
		return static_cast< int16 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int16 -> int8" )
	static FAInt8 CastInt16ToInt8( const FAInt16& I16 )
	{
		return static_cast< int8 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int8 -> int16" )
	static FAInt16 CastInt8ToInt16( const FAInt8& I8 )
	{
		return static_cast< int16 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int16 -> uint8" )
	static uint8 CastInt16ToUInt8( const FAInt16& I16 )
	{
		return static_cast< uint8 >( I16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint8 -> int16" )
	static FAInt16 CastUInt8ToInt16( uint8 UI8 )
	{
		return static_cast< int16 >( UI8 );
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint16 -> int8" )
	static FAInt8 CastUInt16ToInt8( const FAUInt16& UI16 )
	{
		return static_cast< int8 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int8 -> uint16" )
	static FAUInt16 CastInt8ToUInt16( const FAInt8& I8 )
	{
		return static_cast< uint16 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint16 -> uint8" )
	static uint8 CastUInt16ToUInt8( const FAUInt16& UI16 )
	{
		return static_cast< uint8 >( UI16.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint8 -> uint16" )
	static FAUInt16 CastUInt8ToUInt16( uint8 UI8 )
	{
		return static_cast< uint16 >( UI8 );
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "int8 -> uint8" )
	static uint8 CastInt8ToUInt8( const FAInt8& I8 )
	{
		return static_cast< uint8 >( I8.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, meta = (BlueprintAutocast),  DisplayName = "uint8 -> int8" )
	static FAInt8 CastUInt8ToInt8( uint8 UI8 )
	{
		return static_cast< int8 >( UI8 );
	}


	/********/
	/* Plus */
	/********/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + dbl" )
	static FADouble UE4FTDoublePlusDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value + D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + flt" )
	static FADouble UE4FTDoublePlusFloat( const FADouble& D, float F )
	{
		return D.Value + F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + dbl" )
	static FADouble UE4FTFloatPlusDouble( float F, const FADouble& D )
	{
		return F + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + int64" )
	static FADouble UE4FTDoublePlusInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + dbl" )
	static FADouble FInt64PlusDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + uint64" )
	static FADouble UE4FTDoublePlusUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + dbl" )
	static FADouble UE4FTUInt64PlusDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + int32" )
	static FADouble UE4FTDoublePlusInt32( const FADouble& D, int32 I32 )
	{
		return D.Value + I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + dbl" )
	static FADouble UE4FTInt32PlusDouble( int32 I32, const FADouble& D )
	{
		return I32 + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + uint32" )
	static FADouble UE4FTDoublePlusUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + dbl" )
	static FADouble FUInt32PlusDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + int16" )
	static FADouble UE4FTDoublePlusInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + dbl" )
	static FADouble FInt16PlusDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + uint16" )
	static FADouble UE4FTDoublePlusUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + dbl" )
	static FADouble FUInt16PlusDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + int8" )
	static FADouble UE4FTDoublePlusInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + dbl" )
	static FADouble FInt8PlusDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value + D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl + uint8" )
	static FADouble UE4FTDoublePlusUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + dbl" )
	static FADouble UE4FTUInt8PlusDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 + D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + int64" )
	static float UE4FTFloatPlusInt64( float F, const FAInt64& I64 )
	{
		return F + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + flt" )
	static float FInt64PlusFloat( const FAInt64& I64, float F )
	{
		return I64.Value + F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + uint64" )
	static float UE4FTFloatPlusUInt64( float F, const FAUInt64& UI64 )
	{
		return F + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + flt" )
	static float UE4FTUInt64PlusFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value + F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + uint32" )
	static float UE4FTFloatPlusUInt32( float F, const FAUInt32& UI32 )
	{
		return F + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + flt" )
	static float FUInt32PlusFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value + F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + int16" )
	static float UE4FTFloatPlusInt16( float F, const FAInt16& I16 )
	{
		return F + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + flt" )
	static float FInt16PlusFloat( const FAInt16& I16, float F )
	{
		return I16.Value + F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + uint16" )
	static float UE4FTFloatPlusUInt16( float F, const FAUInt16& UI16 )
	{
		return F + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + flt" )
	static float FUInt16PlusFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value + F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt + int8" )
	static float UE4FTFloatPlusInt8( float F, const FAInt8& I8 )
	{
		return F + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + flt" )
	static float FInt8PlusFloat( const FAInt8& I8, float F )
	{
		return I8.Value + F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + int64" )
	static FAInt64 FInt64PlusInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value + I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + uint64" )
	static FAInt64 FInt64PlusUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		return I64.Value + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + int64" )
	static FAInt64 UE4FTUInt64PlusInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		return UI64.Value + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + int32" )
	static FAInt64 FInt64PlusInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value + I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + int64" )
	static FAInt64 UE4FTInt32PlusInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + uint32" )
	static FAInt64 FInt64PlusUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + int64" )
	static FAInt64 FUInt32PlusInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + int16" )
	static FAInt64 FInt64PlusInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + int64" )
	static FAInt64 FInt16PlusInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + uint16" )
	static FAInt64 FInt64PlusUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + int64" )
	static FAInt64 FUInt16PlusInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + int8" )
	static FAInt64 FInt64PlusInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + int64" )
	static FAInt64 FInt8PlusInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value + I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 + uint8" )
	static FAInt64 FInt64PlusUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + int64" )
	static FAInt64 UE4FTUInt8PlusInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 + I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + uint64" )
	static FAUInt64 UE4FTUInt64PlusUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value + UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + int32" )
	static FAUInt64 UE4FTUInt64PlusInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value + I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + uint64" )
	static FAUInt64 UE4FTUInt32PlusInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + uint32" )
	static FAUInt64 UE4FTUInt64PlusUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + uint64" )
	static FAUInt64 UE4FTUUInt32PlusInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + int16" )
	static FAUInt64 UE4FTUInt64PlusInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + uint64" )
	static FAUInt64 FInt16PlusUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + uint16" )
	static FAUInt64 UE4FTUInt64PlusUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + uint64" )
	static FAUInt64 FUInt16PlusUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + int8" )
	static FAUInt64 UE4FTUInt64PlusInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + uint64" )
	static FAUInt64 FInt8PlusUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value + UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 + uint8" )
	static FAUInt64 UE4FTUInt64PlusUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + uint64" )
	static FAUInt64 UE4FTUInt8PlusUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 + UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + uint32" )
	static int32 UE4FTInt32PlusUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + int32" )
	static int32 FUInt32PlusInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32.Value + I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + int16" )
	static int32 UE4FTInt32PlusInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + int32" )
	static int32 FInt16PlusInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value + I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + uint16" )
	static int32 UE4FTInt32PlusUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + int32" )
	static int32 FUInt16PlusInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value + I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 + int8" )
	static int32 UE4FTInt32PlusInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + int32" )
	static int32 FInt8PluInt32s( const FAInt8& I8, int32 I32 )
	{
		return I8.Value + I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + uint32" )
	static FAUInt32 FUInt32PlusUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value + UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + int16" )
	static FAUInt32 FUInt32PlusInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32.Value + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + uint32" )
	static FAUInt32 FInt16PlusUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + uint16" )
	static FAUInt32 FUInt32PlusUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + uint32" )
	static FAUInt32 FUInt16PlusUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + int8" )
	static FAUInt32 FUInt32PlusInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32.Value + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + uint32" )
	static FAUInt32 FInt8PlusUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value + UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 + uint8" )
	static FAUInt32 FUInt32PlusUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + uint32" )
	static FAUInt32 UE4FTUInt8PlusUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 + UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + int16" )
	static FAInt16 FInt16PlusInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value + I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + uint16" )
	static FAInt16 FInt16PlusUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + int16" )
	static FAInt16 FUInt16PlusInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + int8" )
	static FAInt16 FInt16PlusInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + int16" )
	static FAInt16 FInt8PlusInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value + I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 + uint8" )
	static FAInt16 FInt16PlusUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + int16" )
	static FAInt16 UE4FTUInt8PlusInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 + I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + uint16" )
	static FAUInt16 FUInt16PlusUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value + UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + int8" )
	static FAUInt16 FUInt16PlusInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value + I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + uint16" )
	static FAUInt16 FInt8PlusUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value + UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 + uint8" )
	static FAUInt16 FUInt16PlusUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + uint16" )
	static FAUInt16 UE4FTUInt8PlusUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 + UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + int8" )
	static FAInt8 FInt8PlusInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value + I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 + uint8" )
	static FAInt8 FInt8PlusUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value + UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 + int8" )
	static FAInt8 UE4FTUInt8PlusInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 + I8.Value;
	}


	/************/
	/* Subtract */
	/************/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - dbl" )
	static FADouble UE4FTDoubleSubtractDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value - D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - flt" )
	static FADouble UE4FTDoubleSubtractFloat( const FADouble& D, float F )
	{
		return D.Value - F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - dbl" )
	static FADouble UE4FTFloatSubtractDouble( float F, const FADouble& D )
	{
		return F - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - int64" )
	static FADouble UE4FTDoubleSubtractInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - dbl" )
	static FADouble FInt64SubtractDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - uint64" )
	static FADouble UE4FTDoubleSubtractUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - dbl" )
	static FADouble UE4FTUInt64SubtractDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - int32" )
	static FADouble UE4FTDoubleSubtractInt32( const FADouble& D, int32 I32 )
	{
		return D.Value - I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - dbl" )
	static FADouble UE4FTInt32SubtractDouble( int32 I32, const FADouble& D )
	{
		return I32 - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - uint32" )
	static FADouble UE4FTDoubleSubtractUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - dbl" )
	static FADouble FUInt32SubtractDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - int16" )
	static FADouble UE4FTDoubleSubtractInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - dbl" )
	static FADouble FInt16SubtractDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - uint16" )
	static FADouble UE4FTDoubleSubtractUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - dbl" )
	static FADouble FUInt16SubtractDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - int8" )
	static FADouble UE4FTDoubleSubtractInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - dbl" )
	static FADouble FInt8SubtractDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value - D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl - uint8" )
	static FADouble UE4FTDoubleSubtractUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - dbl" )
	static FADouble UE4FTUInt8SubtractDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 - D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - int64" )
	static float UE4FTFloatSubtractInt64( float F, const FAInt64& I64 )
	{
		return F - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - flt" )
	static float FInt64SubtractFloat( const FAInt64& I64, float F )
	{
		return I64.Value - F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - uint64" )
	static float UE4FTFloatSubtractUInt64( float F, const FAUInt64& UI64 )
	{
		return F - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - flt" )
	static float UE4FTUInt64SubtractFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value - F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - uint32" )
	static float UE4FTFloatSubtractUInt32( float F, const FAUInt32& UI32 )
	{
		return F - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - flt" )
	static float FUInt32SubtractFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value - F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - int16" )
	static float UE4FTFloatSubtractInt16( float F, const FAInt16& I16 )
	{
		return F - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - flt" )
	static float FInt16SubtractFloat( const FAInt16& I16, float F )
	{
		return I16.Value - F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - uint16" )
	static float UE4FTFloatSubtractUInt16( float F, const FAUInt16& UI16 )
	{
		return F - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - flt" )
	static float FUInt16SubtractFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value - F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt - int8" )
	static float UE4FTFloatSubtractInt8( float F, const FAInt8& I8 )
	{
		return F - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - flt" )
	static float FInt8SubtractFloat( const FAInt8& I8, float F )
	{
		return I8.Value - F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - int64" )
	static FAInt64 FInt64SubtractInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value - I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - uint64" )
	static FAInt64 FInt64SubtractUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		return I64.Value - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - int64" )
	static FAInt64 UE4FTUInt64SubtractInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		return UI64.Value - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - int32" )
	static FAInt64 FInt64SubtractInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value - I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - int64" )
	static FAInt64 UE4FTInt32SubtractInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - uint32" )
	static FAInt64 FInt64SubtractUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - int64" )
	static FAInt64 FUInt32SubtractInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - int16" )
	static FAInt64 FInt64SubtractInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - int64" )
	static FAInt64 FInt16SubtractInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - uint16" )
	static FAInt64 FInt64SubtractUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - int64" )
	static FAInt64 FUInt16SubtractInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - int8" )
	static FAInt64 FInt64SubtractInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - int64" )
	static FAInt64 FInt8SubtractInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value - I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 - uint8" )
	static FAInt64 FInt64SubtractUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - int64" )
	static FAInt64 UE4FTUInt8SubtractInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 - I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - uint64" )
	static FAUInt64 UE4FTUInt64SubtractUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value - UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - int32" )
	static FAUInt64 UE4FTUInt64SubtractInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value - I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - uint64" )
	static FAUInt64 UE4FTUInt32SubtractInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - uint32" )
	static FAUInt64 UE4FTUInt64SubtractUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - uint64" )
	static FAUInt64 UE4FTUUInt32SubtractInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - int16" )
	static FAUInt64 UE4FTUInt64SubtractInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - uint64" )
	static FAUInt64 FInt16SubtractUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - uint16" )
	static FAUInt64 UE4FTUInt64SubtractUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - uint64" )
	static FAUInt64 FUInt16SubtractUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - int8" )
	static FAUInt64 UE4FTUInt64SubtractInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - uint64" )
	static FAUInt64 FInt8SubtractUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value - UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 - uint8" )
	static FAUInt64 UE4FTUInt64SubtractUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - uint64" )
	static FAUInt64 UE4FTUInt8SubtractUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 - UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - uint32" )
	static int32 UE4FTInt32SubtractUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - int32" )
	static int32 FUInt32SubtractInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32.Value - I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - int16" )
	static int32 UE4FTInt32SubtractInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - int32" )
	static int32 FInt16SubtractInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value - I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - uint16" )
	static int32 UE4FTInt32SubtractUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - int32" )
	static int32 FUInt16SubtractInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value - I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 - int8" )
	static int32 UE4FTInt32SubtractInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - int32" )
	static int32 FInt8MinusInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value - I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - uint32" )
	static FAUInt32 FUInt32SubtractUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value - UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - int16" )
	static FAUInt32 FUInt32SubtractInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32.Value - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - uint32" )
	static FAUInt32 FInt16SubtractUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - uint16" )
	static FAUInt32 FUInt32SubtractUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - uint32" )
	static FAUInt32 FUInt16SubtractUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - int8" )
	static FAUInt32 FUInt32SubtractInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32.Value - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - uint32" )
	static FAUInt32 FInt8SubtractUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value - UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 - uint8" )
	static FAUInt32 FUInt32SubtractUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - uint32" )
	static FAUInt32 UE4FTUInt8SubtractUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 - UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - int16" )
	static FAInt16 FInt16SubtractInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value - I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - uint16" )
	static FAInt16 FInt16SubtractUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - int16" )
	static FAInt16 FUInt16SubtractInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - int8" )
	static FAInt16 FInt16SubtractInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - int16" )
	static FAInt16 FInt8SubtractInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value - I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 - uint8" )
	static FAInt16 FInt16SubtractUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - int16" )
	static FAInt16 UE4FTUInt8SubtractInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 - I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - uint16" )
	static FAUInt16 FUInt16SubtractUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value - UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - int8" )
	static FAUInt16 FUInt16SubtractInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value - I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - uint16" )
	static FAUInt16 FInt8SubtractUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value - UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 - uint8" )
	static FAUInt16 FUInt16SubtractUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - uint16" )
	static FAUInt16 UE4FTUInt8SubtractUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 - UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - int8" )
	static FAInt8 FInt8SubtractInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value - I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 - uint8" )
	static FAInt8 FInt8SubtractUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value - UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 - int8" )
	static FAInt8 UE4FTUInt8SubtractInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 - I8.Value;
	}


	/*********/
	/* Times */
	/*********/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * dbl" )
	static FADouble UE4FTDoubleTimesDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value * D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * flt" )
	static FADouble UE4FTDoubleTimesFloat( const FADouble& D, float F )
	{
		return D.Value * F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * dbl" )
	static FADouble UE4FTFloatTimesDouble( float F, const FADouble& D )
	{
		return F * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * int64" )
	static FADouble UE4FTDoubleTimesInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * dbl" )
	static FADouble FInt64TimesDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * uint64" )
	static FADouble UE4FTDoubleTimesUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * dbl" )
	static FADouble UE4FTUInt64TimesDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * int32" )
	static FADouble UE4FTDoubleTimesInt32( const FADouble& D, int32 I32 )
	{
		return D.Value * I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * dbl" )
	static FADouble UE4FTInt32TimesDouble( int32 I32, const FADouble& D )
	{
		return I32 * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * uint32" )
	static FADouble UE4FTDoubleTimesUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * dbl" )
	static FADouble FUInt32TimesDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * int16" )
	static FADouble UE4FTDoubleTimesInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * dbl" )
	static FADouble FInt16TimesDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * uint16" )
	static FADouble UE4FTDoubleTimesUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * dbl" )
	static FADouble FUInt16TimesDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * int8" )
	static FADouble UE4FTDoubleTimesInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * dbl" )
	static FADouble FInt8TimesDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value * D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl * uint8" )
	static FADouble UE4FTDoubleTimesUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * dbl" )
	static FADouble UE4FTUInt8TimesDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 * D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * int64" )
	static float UE4FTFloatTimesInt64( float F, const FAInt64& I64 )
	{
		return F * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * flt" )
	static float FInt64TimesFloat( const FAInt64& I64, float F )
	{
		return I64.Value * F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * uint64" )
	static float UE4FTFloatTimesUInt64( float F, const FAUInt64& UI64 )
	{
		return F * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * flt" )
	static float UE4FTUInt64TimesFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value * F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * uint32" )
	static float UE4FTFloatTimesUInt32( float F, const FAUInt32& UI32 )
	{
		return F * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * flt" )
	static float FUInt32TimesFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value * F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * int16" )
	static float UE4FTFloatTimesInt16( float F, const FAInt16& I16 )
	{
		return F * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * flt" )
	static float FInt16TimesFloat( const FAInt16& I16, float F )
	{
		return I16.Value * F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * uint16" )
	static float UE4FTFloatTimesUInt16( float F, const FAUInt16& UI16 )
	{
		return F * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * flt" )
	static float FUInt16TimesFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value * F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt * int8" )
	static float UE4FTFloatTimesInt8( float F, const FAInt8& I8 )
	{
		return F * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * flt" )
	static float FInt8TimesFloat( const FAInt8& I8, float F )
	{
		return I8.Value * F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * int64" )
	static FAInt64 FInt64TimesInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value * I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * uint64" )
	static FAInt64 FInt64TimesUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		return I64.Value * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * int64" )
	static FAInt64 UE4FTUInt64TimesInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		return UI64.Value * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * int32" )
	static FAInt64 FInt64TimesInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value * I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * int64" )
	static FAInt64 UE4FTInt32TimesInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * uint32" )
	static FAInt64 FInt64TimesUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * int64" )
	static FAInt64 FUInt32TimesInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * int16" )
	static FAInt64 FInt64TimesInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * int64" )
	static FAInt64 FInt16TimesInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * uint16" )
	static FAInt64 FInt64TimesUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * int64" )
	static FAInt64 FUInt16TimesInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * int8" )
	static FAInt64 FInt64TimesInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * int64" )
	static FAInt64 FInt8TimesInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value * I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 * uint8" )
	static FAInt64 FInt64TimesUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * int64" )
	static FAInt64 UE4FTUInt8TimesInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 * I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * uint64" )
	static FAUInt64 UE4FTUInt64TimesUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value * UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * int32" )
	static FAUInt64 UE4FTUInt64TimesInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value * I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * uint64" )
	static FAUInt64 UE4FTUInt32TimesInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * uint32" )
	static FAUInt64 UE4FTUInt64TimesUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * uint64" )
	static FAUInt64 UE4FTUUInt32TimesInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * int16" )
	static FAUInt64 UE4FTUInt64TimesInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * uint64" )
	static FAUInt64 FInt16TimesUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * uint16" )
	static FAUInt64 UE4FTUInt64TimesUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * uint64" )
	static FAUInt64 FUInt16TimesUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * int8" )
	static FAUInt64 UE4FTUInt64TimesInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * uint64" )
	static FAUInt64 FInt8TimesUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value * UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 * uint8" )
	static FAUInt64 UE4FTUInt64TimesUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * uint64" )
	static FAUInt64 UE4FTUInt8TimesUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 * UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * uint32" )
	static int32 UE4FTInt32TimesUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * int32" )
	static int32 FUInt32TimesInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32.Value * I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * int16" )
	static int32 UE4FTInt32TimesInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * int32" )
	static int32 FInt16TimesInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value * I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * uint16" )
	static int32 UE4FTInt32TimesUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * int32" )
	static int32 FUInt16TimesInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value * I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 * int8" )
	static int32 UE4FTInt32TimesInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * int32" )
	static int32 FInt8TimesInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value * I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * uint32" )
	static FAUInt32 FUInt32TimesUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value * UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * int16" )
	static FAUInt32 FUInt32TimesInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32.Value * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * uint32" )
	static FAUInt32 FInt16TimesUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * uint16" )
	static FAUInt32 FUInt32TimesUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * uint32" )
	static FAUInt32 FUInt16TimesUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * int8" )
	static FAUInt32 FUInt32TimesInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32.Value * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * uint32" )
	static FAUInt32 FInt8TimesUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value * UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 * uint8" )
	static FAUInt32 FUInt32TimesUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * uint32" )
	static FAUInt32 UE4FTUInt8TimesUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 * UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * int16" )
	static FAInt16 FInt16TimesInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value * I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * uint16" )
	static FAInt16 FInt16TimesUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * int16" )
	static FAInt16 FUInt16TimesInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * int8" )
	static FAInt16 FInt16TimesInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * int16" )
	static FAInt16 FInt8TimesInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value * I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 * uint8" )
	static FAInt16 FInt16TimesUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * int16" )
	static FAInt16 UE4FTUInt8TimesInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 * I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * uint16" )
	static FAUInt16 FUInt16TimesUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value * UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * int8" )
	static FAUInt16 FUInt16TimesInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value * I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * uint16" )
	static FAUInt16 FInt8TimesUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value * UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 * uint8" )
	static FAUInt16 FUInt16TimesUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * uint16" )
	static FAUInt16 UE4FTUInt8TimesUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 * UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * int8" )
	static FAInt8 FInt8TimesInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value * I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 * uint8" )
	static FAInt8 FInt8TimesUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value * UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 * int8" )
	static FAInt8 UE4FTUInt8TimesInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 * I8.Value;
	}


	/**********/
	/* Divide */
	/**********/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / dbl" )
	static FADouble UE4FTDoubleDivideDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value / D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / flt" )
	static FADouble UE4FTDoubleDivideFloat( const FADouble& D, float F )
	{
		return D.Value / F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / dbl" )
	static FADouble UE4FTFloatDivideDouble( float F, const FADouble& D )
	{
		return F / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / int64" )
	static FADouble UE4FTDoubleDivideInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / dbl" )
	static FADouble FInt64DivideDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / uint64" )
	static FADouble UE4FTDoubleDivideUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / dbl" )
	static FADouble UE4FTUInt64DivideDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / int32" )
	static FADouble UE4FTDoubleDivideInt32( const FADouble& D, int32 I32 )
	{
		return D.Value / I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / dbl" )
	static FADouble UE4FTInt32DivideDouble( int32 I32, const FADouble& D )
	{
		return I32 / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / uint32" )
	static FADouble UE4FTDoubleDivideUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / dbl" )
	static FADouble FUInt32DivideDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / int16" )
	static FADouble UE4FTDoubleDivideInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / dbl" )
	static FADouble FInt16DivideDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / uint16" )
	static FADouble UE4FTDoubleDivideUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / dbl" )
	static FADouble FUInt16DivideDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / int8" )
	static FADouble UE4FTDoubleDivideInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / dbl" )
	static FADouble FInt8DivideDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value / D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl / uint8" )
	static FADouble UE4FTDoubleDivideUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / dbl" )
	static FADouble UE4FTUInt8DivideDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 / D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / int64" )
	static float UE4FTFloatDivideInt64( float F, const FAInt64& I64 )
	{
		return F / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / flt" )
	static float FInt64DivideFloat( const FAInt64& I64, float F )
	{
		return I64.Value / F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / uint64" )
	static float UE4FTFloatDivideUInt64( float F, const FAUInt64& UI64 )
	{
		return F / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / flt" )
	static float UE4FTUInt64DivideFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value / F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / uint32" )
	static float UE4FTFloatDivideUInt32( float F, const FAUInt32& UI32 )
	{
		return F / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / flt" )
	static float FUInt32DivideFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value / F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / int16" )
	static float UE4FTFloatDivideInt16( float F, const FAInt16& I16 )
	{
		return F / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / flt" )
	static float FInt16DivideFloat( const FAInt16& I16, float F )
	{
		return I16.Value / F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / uint16" )
	static float UE4FTFloatDivideUInt16( float F, const FAUInt16& UI16 )
	{
		return F / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / flt" )
	static float FUInt16DivideFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value / F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt / int8" )
	static float UE4FTFloatDivideInt8( float F, const FAInt8& I8 )
	{
		return F / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / flt" )
	static float FInt8DivideFloat( const FAInt8& I8, float F )
	{
		return I8.Value / F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / int64" )
	static FAInt64 FInt64DivideInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value / I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / uint64" )
	static FAInt64 FInt64DivideUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		return I64.Value / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / int64" )
	static FAInt64 UE4FTUInt64DivideInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		return UI64.Value / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / int32" )
	static FAInt64 FInt64DivideInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value / I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / int64" )
	static FAInt64 UE4FTInt32DivideInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / uint32" )
	static FAInt64 FInt64DivideUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / int64" )
	static FAInt64 FUInt32DivideInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / int16" )
	static FAInt64 FInt64DivideInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / int64" )
	static FAInt64 FInt16DivideInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / uint16" )
	static FAInt64 FInt64DivideUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / int64" )
	static FAInt64 FUInt16DivideInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / int8" )
	static FAInt64 FInt64DivideInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / int64" )
	static FAInt64 FInt8DivideInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value / I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 / uint8" )
	static FAInt64 FInt64DivideUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / int64" )
	static FAInt64 UE4FTUInt8DivideInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 / I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / uint64" )
	static FAUInt64 UE4FTUInt64DivideUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value / UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / int32" )
	static FAUInt64 UE4FTUInt64DivideInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value / I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / uint64" )
	static FAUInt64 UE4FTUInt32DivideInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / uint32" )
	static FAUInt64 UE4FTUInt64DivideUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / uint64" )
	static FAUInt64 UE4FTUUInt32DivideInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / int16" )
	static FAUInt64 UE4FTUInt64DivideInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / uint64" )
	static FAUInt64 FInt16DivideUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / uint16" )
	static FAUInt64 UE4FTUInt64DivideUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / uint64" )
	static FAUInt64 FUInt16DivideUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / int8" )
	static FAUInt64 UE4FTUInt64DivideInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / uint64" )
	static FAUInt64 FInt8DivideUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value / UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 / uint8" )
	static FAUInt64 UE4FTUInt64DivideUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / uint64" )
	static FAUInt64 UE4FTUInt8DivideUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 / UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / uint32" )
	static int32 UE4FTInt32DivideUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / int32" )
	static int32 FUInt32DivideInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32.Value / I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / int16" )
	static int32 UE4FTInt32DivideInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / int32" )
	static int32 FInt16DivideInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value / I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / uint16" )
	static int32 UE4FTInt32DivideUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / int32" )
	static int32 FUInt16DivideInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value / I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 / int8" )
	static int32 UE4FTInt32DivideInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / int32" )
	static int32 FInt8DivideInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value / I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / uint32" )
	static FAUInt32 FUInt32DivideUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value / UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / int16" )
	static FAUInt32 FUInt32DivideInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32.Value / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / uint32" )
	static FAUInt32 FInt16DivideUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / uint16" )
	static FAUInt32 FUInt32DivideUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / uint32" )
	static FAUInt32 FUInt16DivideUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / int8" )
	static FAUInt32 FUInt32DivideInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32.Value / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / uint32" )
	static FAUInt32 FInt8DivideUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value / UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 / uint8" )
	static FAUInt32 FUInt32DivideUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / uint32" )
	static FAUInt32 UE4FTUInt8DivideUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 / UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / int16" )
	static FAInt16 FInt16DivideInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value / I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / uint16" )
	static FAInt16 FInt16DivideUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / int16" )
	static FAInt16 FUInt16DivideInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / int8" )
	static FAInt16 FInt16DivideInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / int16" )
	static FAInt16 FInt8DivideInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value / I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 / uint8" )
	static FAInt16 FInt16DivideUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / int16" )
	static FAInt16 UE4FTUInt8DivideInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 / I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / uint16" )
	static FAUInt16 FUInt16DivideUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value / UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / int8" )
	static FAUInt16 FUInt16DivideInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value / I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / uint16" )
	static FAUInt16 FInt8DivideUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value / UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 / uint8" )
	static FAUInt16 FUInt16DivideUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / uint16" )
	static FAUInt16 UE4FTUInt8DivideUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 / UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / int8" )
	static FAInt8 FInt8DivideInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value / I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 / uint8" )
	static FAInt8 FInt8DivideUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value / UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 / int8" )
	static FAInt8 UE4FTUInt8DivideInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 / I8.Value;
	}


	/**********/
	/* Equals */
	/**********/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == dbl" )
	static bool UE4FTDoubleEqualsDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value == D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == flt" )
	static bool UE4FTDoubleEqualsFloat( const FADouble& D, float F )
	{
		return D.Value == F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == dbl" )
	static bool UE4FTFloatEqualsDouble( float F, const FADouble& D )
	{
		return F == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == int64" )
	static bool UE4FTDoubleEqualsInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == dbl" )
	static bool FInt64EqualsDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == uint64" )
	static bool UE4FTDoubleEqualsUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == dbl" )
	static bool UE4FTUInt64EqualsDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == int32" )
	static bool UE4FTDoubleEqualsInt32( const FADouble& D, int32 I32 )
	{
		return D.Value == I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == dbl" )
	static bool UE4FTInt32EqualsDouble( int32 I32, const FADouble& D )
	{
		return I32 == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == uint32" )
	static bool UE4FTDoubleEqualsUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == dbl" )
	static bool FUInt32EqualsDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == int16" )
	static bool UE4FTDoubleEqualsInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == dbl" )
	static bool FInt16EqualsDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == uint16" )
	static bool UE4FTDoubleEqualsUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == dbl" )
	static bool FUInt16EqualsDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == int8" )
	static bool UE4FTDoubleEqualsInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == dbl" )
	static bool FInt8EqualsDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value == D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl == uint8" )
	static bool UE4FTDoubleEqualsUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == dbl" )
	static bool UE4FTUInt8EqualsDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 == D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == int64" )
	static bool UE4FTFloatEqualsInt64( float F, const FAInt64& I64 )
	{
		return F == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == flt" )
	static bool FInt64EqualsFloat( const FAInt64& I64, float F )
	{
		return I64.Value == F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == uint64" )
	static bool UE4FTFloatEqualsUInt64( float F, const FAUInt64& UI64 )
	{
		return F == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == flt" )
	static bool UE4FTUInt64EqualsFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value == F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == uint32" )
	static bool UE4FTFloatEqualsUInt32( float F, const FAUInt32& UI32 )
	{
		return F == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == flt" )
	static bool FUInt32EqualsFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value == F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == int16" )
	static bool UE4FTFloatEqualsInt16( float F, const FAInt16& I16 )
	{
		return F == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == flt" )
	static bool FInt16EqualsFloat( const FAInt16& I16, float F )
	{
		return I16.Value == F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == uint16" )
	static bool UE4FTFloatEqualsUInt16( float F, const FAUInt16& UI16 )
	{
		return F == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == flt" )
	static bool FUInt16EqualsFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value == F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt == int8" )
	static bool UE4FTFloatEqualsInt8( float F, const FAInt8& I8 )
	{
		return F == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == flt" )
	static bool FInt8EqualsFloat( const FAInt8& I8, float F )
	{
		return I8.Value == F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == int64" )
	static bool FInt64EqualsInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value == I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == uint64" )
	static bool FInt64EqualsUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		return I64.Value == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == int64" )
	static bool UE4FTUInt64EqualsInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		return UI64.Value == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == int32" )
	static bool FInt64EqualsInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value == I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == int64" )
	static bool UE4FTInt32EqualsInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == uint32" )
	static bool FInt64EqualsUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == int64" )
	static bool FUInt32EqualsInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == int16" )
	static bool FInt64EqualsInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == int64" )
	static bool FInt16EqualsInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == uint16" )
	static bool FInt64EqualsUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == int64" )
	static bool FUInt16EqualsInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == int8" )
	static bool FInt64EqualsInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == int64" )
	static bool FInt8EqualsInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value == I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 == uint8" )
	static bool FInt64EqualsUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == int64" )
	static bool UE4FTUInt8EqualsInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 == I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == uint64" )
	static bool UE4FTUInt64EqualsUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value == UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == int32" )
	static bool UE4FTUInt64EqualsInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value == I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == uint64" )
	static bool UE4FTUInt32EqualsInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == uint32" )
	static bool UE4FTUInt64EqualsUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == uint64" )
	static bool UE4FTUUInt32EqualsInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == int16" )
	static bool UE4FTUInt64EqualsInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == uint64" )
	static bool FInt16EqualsUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == uint16" )
	static bool UE4FTUInt64EqualsUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == uint64" )
	static bool FUInt16EqualsUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == int8" )
	static bool UE4FTUInt64EqualsInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == uint64" )
	static bool FInt8EqualsUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value == UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 == uint8" )
	static bool UE4FTUInt64EqualsUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == uint64" )
	static bool UE4FTUInt8EqualsUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 == UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == uint32" )
	static bool UE4FTInt32EqualsUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == int32" )
	static bool FUInt32EqualsInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32.Value == I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == int16" )
	static bool UE4FTInt32EqualsInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == int32" )
	static bool FInt16EqualsInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value == I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == uint16" )
	static bool UE4FTInt32EqualsUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == int32" )
	static bool FUInt16EqualsInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value == I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 == int8" )
	static bool UE4FTInt32EqualsInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == int32" )
	static bool FInt8EqualsInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value == I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == uint32" )
	static bool FUInt32EqualsUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value == UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == int16" )
	static bool FUInt32EqualsInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32.Value == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == uint32" )
	static bool FInt16EqualsUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == uint16" )
	static bool FUInt32EqualsUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == uint32" )
	static bool FUInt16EqualsUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == int8" )
	static bool FUInt32EqualsInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32.Value == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == uint32" )
	static bool FInt8EqualsUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value == UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 == uint8" )
	static bool FUInt32EqualsUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == uint32" )
	static bool UE4FTUInt8EqualsUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 == UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == int16" )
	static bool FInt16EqualsInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value == I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == uint16" )
	static bool FInt16EqualsUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == int16" )
	static bool FUInt16EqualsInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == int8" )
	static bool FInt16EqualsInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == int16" )
	static bool FInt8EqualsInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value == I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 == uint8" )
	static bool FInt16EqualsUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == int16" )
	static bool UE4FTUInt8EqualsInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 == I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == uint16" )
	static bool FUInt16EqualsUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value == UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == int8" )
	static bool FUInt16EqualsInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value == I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == uint16" )
	static bool FInt8EqualsUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value == UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 == uint8" )
	static bool FUInt16EqualsUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == uint16" )
	static bool UE4FTUInt8EqualsUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 == UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == int8" )
	static bool FInt8EqualsInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value == I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 == uint8" )
	static bool FInt8EqualsUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value == UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 == int8" )
	static bool UE4FTUInt8EqualsInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 == I8.Value;
	}


	/*************/
	/* NotEquals */
	/*************/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != dbl" )
	static bool UE4FTDoubleNotEqualsDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value != D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != flt" )
	static bool UE4FTDoubleNotEqualsFloat( const FADouble& D, float F )
	{
		return D.Value != F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != dbl" )
	static bool UE4FTFloatNotEqualsDouble( float F, const FADouble& D )
	{
		return F != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != int64" )
	static bool UE4FTDoubleNotEqualsInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != dbl" )
	static bool FInt64NotEqualsDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != uint64" )
	static bool UE4FTDoubleNotEqualsUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != dbl" )
	static bool UE4FTUInt64NotEqualsDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != int32" )
	static bool UE4FTDoubleNotEqualsInt32( const FADouble& D, int32 I32 )
	{
		return D.Value != I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != dbl" )
	static bool UE4FTInt32NotEqualsDouble( int32 I32, const FADouble& D )
	{
		return I32 != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != uint32" )
	static bool UE4FTDoubleNotEqualsUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != dbl" )
	static bool FUInt32NotEqualsDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != int16" )
	static bool UE4FTDoubleNotEqualsInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != dbl" )
	static bool FInt16NotEqualsDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != uint16" )
	static bool UE4FTDoubleNotEqualsUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != dbl" )
	static bool FUInt16NotEqualsDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != int8" )
	static bool UE4FTDoubleNotEqualsInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != dbl" )
	static bool FInt8NotEqualsDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value != D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl != uint8" )
	static bool UE4FTDoubleNotEqualsUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != dbl" )
	static bool UE4FTUInt8NotEqualsDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 != D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != int64" )
	static bool UE4FTFloatNotEqualsInt64( float F, const FAInt64& I64 )
	{
		return F != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != flt" )
	static bool FInt64NotEqualsFloat( const FAInt64& I64, float F )
	{
		return I64.Value != F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != uint64" )
	static bool UE4FTFloatNotEqualsUInt64( float F, const FAUInt64& UI64 )
	{
		return F != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != flt" )
	static bool UE4FTUInt64NotEqualsFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value != F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != uint32" )
	static bool UE4FTFloatNotEqualsUInt32( float F, const FAUInt32& UI32 )
	{
		return F != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != flt" )
	static bool FUInt32NotEqualsFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value != F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != int16" )
	static bool UE4FTFloatNotEqualsInt16( float F, const FAInt16& I16 )
	{
		return F != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != flt" )
	static bool FInt16NotEqualsFloat( const FAInt16& I16, float F )
	{
		return I16.Value != F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != uint16" )
	static bool UE4FTFloatNotEqualsUInt16( float F, const FAUInt16& UI16 )
	{
		return F != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != flt" )
	static bool FUInt16NotEqualsFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value != F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt != int8" )
	static bool UE4FTFloatNotEqualsInt8( float F, const FAInt8& I8 )
	{
		return F != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != flt" )
	static bool FInt8NotEqualsFloat( const FAInt8& I8, float F )
	{
		return I8.Value != F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != int64" )
	static bool FInt64NotEqualsInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value != I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != uint64" )
	static bool FInt64NotEqualsUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		return I64.Value != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != int64" )
	static bool UE4FTUInt64NotEqualsInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		return UI64.Value != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != int32" )
	static bool FInt64NotEqualsInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value != I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != int64" )
	static bool UE4FTInt32NotEqualsInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != uint32" )
	static bool FInt64NotEqualsUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != int64" )
	static bool FUInt32NotEqualsInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != int16" )
	static bool FInt64NotEqualsInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != int64" )
	static bool FInt16NotEqualsInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != uint16" )
	static bool FInt64NotEqualsUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != int64" )
	static bool FUInt16NotEqualsInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != int8" )
	static bool FInt64NotEqualsInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != int64" )
	static bool FInt8NotEqualsInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value != I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 != uint8" )
	static bool FInt64NotEqualsUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != int64" )
	static bool UE4FTUInt8NotEqualsInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 != I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != uint64" )
	static bool UE4FTUInt64NotEqualsUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value != UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != int32" )
	static bool UE4FTUInt64NotEqualsInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value != I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != uint64" )
	static bool UE4FTUInt32NotEqualsInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != uint32" )
	static bool UE4FTUInt64NotEqualsUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != uint64" )
	static bool UE4FTUUInt32NotEqualsInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != int16" )
	static bool UE4FTUInt64NotEqualsInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != uint64" )
	static bool FInt16NotEqualsUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != uint16" )
	static bool UE4FTUInt64NotEqualsUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != uint64" )
	static bool FUInt16NotEqualsUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != int8" )
	static bool UE4FTUInt64NotEqualsInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != uint64" )
	static bool FInt8NotEqualsUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value != UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 != uint8" )
	static bool UE4FTUInt64NotEqualsUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != uint64" )
	static bool UE4FTUInt8NotEqualsUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 != UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != uint32" )
	static bool UE4FTInt32NotEqualsUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != int32" )
	static bool FUInt32NotEqualsInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32.Value != I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != int16" )
	static bool UE4FTInt32NotEqualsInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != int32" )
	static bool FInt16NotEqualsInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value != I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != uint16" )
	static bool UE4FTInt32NotEqualsUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != int32" )
	static bool FUInt16NotEqualsInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value != I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 != int8" )
	static bool UE4FTInt32NotEqualsInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != int32" )
	static bool FInt8NotEqualsInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value != I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != uint32" )
	static bool FUInt32NotEqualsUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value != UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != int16" )
	static bool FUInt32NotEqualsInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32.Value != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != uint32" )
	static bool FInt16NotEqualsUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != uint16" )
	static bool FUInt32NotEqualsUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != uint32" )
	static bool FUInt16NotEqualsUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != int8" )
	static bool FUInt32NotEqualsInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32.Value != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != uint32" )
	static bool FInt8NotEqualsUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value != UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 != uint8" )
	static bool FUInt32NotEqualsUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != uint32" )
	static bool UE4FTUInt8NotEqualsUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 != UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != int16" )
	static bool FInt16NotEqualsInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value != I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != uint16" )
	static bool FInt16NotEqualsUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != int16" )
	static bool FUInt16NotEqualsInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != int8" )
	static bool FInt16NotEqualsInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != int16" )
	static bool FInt8NotEqualsInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value != I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 != uint8" )
	static bool FInt16NotEqualsUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != int16" )
	static bool UE4FTUInt8NotEqualsInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 != I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != uint16" )
	static bool FUInt16NotEqualsUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value != UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != int8" )
	static bool FUInt16NotEqualsInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value != I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != uint16" )
	static bool FInt8NotEqualsUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value != UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 != uint8" )
	static bool FUInt16NotEqualsUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != uint16" )
	static bool UE4FTUInt8NotEqualsUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 != UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != int8" )
	static bool FInt8NotEqualsInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value != I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 != uint8" )
	static bool FInt8NotEqualsUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value != UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 != int8" )
	static bool UE4FTUInt8NotEqualsInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 != I8.Value;
	}


	/***************/
	/* GreaterThan */
	/***************/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > dbl" )
	static bool UE4FTDoubleGreaterThanDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value > D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > flt" )
	static bool UE4FTDoubleGreaterThanFloat( const FADouble& D, float F )
	{
		return D.Value > F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > dbl" )
	static bool UE4FTFloatGreaterThanDouble( float F, const FADouble& D )
	{
		return F > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > int64" )
	static bool UE4FTDoubleGreaterThanInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > dbl" )
	static bool FInt64GreaterThanDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > uint64" )
	static bool UE4FTDoubleGreaterThanUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > dbl" )
	static bool UE4FTUInt64GreaterThanDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > int32" )
	static bool UE4FTDoubleGreaterThanInt32( const FADouble& D, int32 I32 )
	{
		return D.Value > I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > dbl" )
	static bool UE4FTInt32GreaterThanDouble( int32 I32, const FADouble& D )
	{
		return I32 > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > uint32" )
	static bool UE4FTDoubleGreaterThanUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value > UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > dbl" )
	static bool FUInt32GreaterThanDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > int16" )
	static bool UE4FTDoubleGreaterThanInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > dbl" )
	static bool FInt16GreaterThanDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > uint16" )
	static bool UE4FTDoubleGreaterThanUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > dbl" )
	static bool FUInt16GreaterThanDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > int8" )
	static bool UE4FTDoubleGreaterThanInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > dbl" )
	static bool FInt8GreaterThanDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value > D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl > uint8" )
	static bool UE4FTDoubleGreaterThanUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > dbl" )
	static bool UE4FTUInt8GreaterThanDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 > D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > int64" )
	static bool UE4FTFloatGreaterThanInt64( float F, const FAInt64& I64 )
	{
		return F > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > flt" )
	static bool FInt64GreaterThanFloat( const FAInt64& I64, float F )
	{
		return I64.Value > F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > uint64" )
	static bool UE4FTFloatGreaterThanUInt64( float F, const FAUInt64& UI64 )
	{
		return F > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > flt" )
	static bool UE4FTUInt64GreaterThanFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value > F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > uint32" )
	static bool UE4FTFloatGreaterThanUInt32( float F, const FAUInt32& UI32 )
	{
		return F > UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > flt" )
	static bool FUInt32GreaterThanFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value > F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > int16" )
	static bool UE4FTFloatGreaterThanInt16( float F, const FAInt16& I16 )
	{
		return F > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > flt" )
	static bool FInt16GreaterThanFloat( const FAInt16& I16, float F )
	{
		return I16.Value > F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > uint16" )
	static bool UE4FTFloatGreaterThanUInt16( float F, const FAUInt16& UI16 )
	{
		return F > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > flt" )
	static bool FUInt16GreaterThanFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value > F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt > int8" )
	static bool UE4FTFloatGreaterThanInt8( float F, const FAInt8& I8 )
	{
		return F > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > flt" )
	static bool FInt8GreaterThanFloat( const FAInt8& I8, float F )
	{
		return I8.Value > F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > int64" )
	static bool FInt64GreaterThanInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value > I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > uint64" )
	static bool FInt64GreaterThanUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		if ( I64.Value < 0 )
			return false;

		return static_cast< uint64 >( I64.Value ) > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > int64" )
	static bool UE4FTUInt64GreaterThanInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		if ( I64.Value < 0 )
			return true;

		return UI64.Value > static_cast< uint64 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > int32" )
	static bool FInt64GreaterThanInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value > I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > int64" )
	static bool UE4FTInt32GreaterThanInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > uint32" )
	static bool FInt64GreaterThanUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value > UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > int64" )
	static bool FUInt32GreaterThanInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > int16" )
	static bool FInt64GreaterThanInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > int64" )
	static bool FInt16GreaterThanInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > uint16" )
	static bool FInt64GreaterThanUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > int64" )
	static bool FUInt16GreaterThanInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > int8" )
	static bool FInt64GreaterThanInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > int64" )
	static bool FInt8GreaterThanInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value > I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 > uint8" )
	static bool FInt64GreaterThanUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > int64" )
	static bool UE4FTUInt8GreaterThanInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 > I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > uint64" )
	static bool UE4FTUInt64GreaterThanUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value > UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > int32" )
	static bool UE4FTUInt64GreaterThanInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value > I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > uint64" )
	static bool UE4FTUInt32GreaterThanInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > uint32" )
	static bool UE4FTUInt64GreaterThanUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value > UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > uint64" )
	static bool UE4FTUUInt32GreaterThanInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > int16" )
	static bool UE4FTUInt64GreaterThanInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > uint64" )
	static bool FInt16GreaterThanUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > uint16" )
	static bool UE4FTUInt64GreaterThanUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > uint64" )
	static bool FUInt16GreaterThanUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > int8" )
	static bool UE4FTUInt64GreaterThanInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > uint64" )
	static bool FInt8GreaterThanUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value > UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 > uint8" )
	static bool UE4FTUInt64GreaterThanUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > uint64" )
	static bool UE4FTUInt8GreaterThanUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 > UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > uint32" )
	static bool UE4FTInt32GreaterThanUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 > UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > int32" )
	static bool FUInt32GreaterThanInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32 > I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > int16" )
	static bool UE4FTInt32GreaterThanInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > int32" )
	static bool FInt16GreaterThanInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value > I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > uint16" )
	static bool UE4FTInt32GreaterThanUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > int32" )
	static bool FUInt16GreaterThanInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value > I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 > int8" )
	static bool UE4FTInt32GreaterThanInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > int32" )
	static bool FInt8GreaterThannt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value > I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > uint32" )
	static bool FUInt32GreaterThanUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value > UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > int16" )
	static bool FUInt32GreaterThanInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32 > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > uint32" )
	static bool FInt16GreaterThanUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value > UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > uint16" )
	static bool FUInt32GreaterThanUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > uint32" )
	static bool FUInt16GreaterThanUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value > UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > int8" )
	static bool FUInt32GreaterThanInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32 > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > uint32" )
	static bool FInt8GreaterThanUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value > UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 > uint8" )
	static bool FUInt32GreaterThanUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > uint32" )
	static bool UE4FTUInt8GreaterThanUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 > UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > int16" )
	static bool FInt16GreaterThanInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value > I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > uint16" )
	static bool FInt16GreaterThanUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > int16" )
	static bool FUInt16GreaterThanInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > int8" )
	static bool FInt16GreaterThanInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > int16" )
	static bool FInt8GreaterThanInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value > I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 > uint8" )
	static bool FInt16GreaterThanUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > int16" )
	static bool UE4FTUInt8GreaterThanInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 > I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > uint16" )
	static bool FUInt16GreaterThanUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value > UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > int8" )
	static bool FUInt16GreaterThanInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value > I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > uint16" )
	static bool FInt8GreaterThanUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value > UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 > uint8" )
	static bool FUInt16GreaterThanUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > uint16" )
	static bool UE4FTUInt8GreaterThanUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 > UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > int8" )
	static bool FInt8GreaterThanInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value > I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 > uint8" )
	static bool FInt8GreaterThanUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value > UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 > int8" )
	static bool UE4FTUInt8GreaterThanInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 > I8.Value;
	}


	/************/
	/* LessThan */
	/***********/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < dbl" )
	static bool UE4FTDoubleLessThanDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value < D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < flt" )
	static bool UE4FTDoubleLessThanFloat( const FADouble& D, float F )
	{
		return D.Value < F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < dbl" )
	static bool UE4FTFloatLessThanDouble( float F, const FADouble& D )
	{
		return F < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < int64" )
	static bool UE4FTDoubleLessThanInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < dbl" )
	static bool FInt64LessThanDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < uint64" )
	static bool UE4FTDoubleLessThanUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < dbl" )
	static bool UE4FTUInt64LessThanDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < int32" )
	static bool UE4FTDoubleLessThanInt32( const FADouble& D, int32 I32 )
	{
		return D.Value < I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < dbl" )
	static bool UE4FTInt32LessThanDouble( int32 I32, const FADouble& D )
	{
		return I32 < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < uint32" )
	static bool UE4FTDoubleLessThanUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value < UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < dbl" )
	static bool FUInt32LessThanDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < int16" )
	static bool UE4FTDoubleLessThanInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < dbl" )
	static bool FInt16LessThanDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < uint16" )
	static bool UE4FTDoubleLessThanUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < dbl" )
	static bool FUInt16LessThanDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < int8" )
	static bool UE4FTDoubleLessThanInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < dbl" )
	static bool FInt8LessThanDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value < D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl < uint8" )
	static bool UE4FTDoubleLessThanUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < dbl" )
	static bool UE4FTUInt8LessThanDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 < D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < int64" )
	static bool UE4FTFloatLessThanInt64( float F, const FAInt64& I64 )
	{
		return F < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < flt" )
	static bool FInt64LessThanFloat( const FAInt64& I64, float F )
	{
		return I64.Value < F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < uint64" )
	static bool UE4FTFloatLessThanUInt64( float F, const FAUInt64& UI64 )
	{
		return F < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < flt" )
	static bool UE4FTUInt64LessThanFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value < F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < uint32" )
	static bool UE4FTFloatLessThanUInt32( float F, const FAUInt32& UI32 )
	{
		return F < UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < flt" )
	static bool FUInt32LessThanFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value < F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < int16" )
	static bool UE4FTFloatLessThanInt16( float F, const FAInt16& I16 )
	{
		return F < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < flt" )
	static bool FInt16LessThanFloat( const FAInt16& I16, float F )
	{
		return I16.Value < F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < uint16" )
	static bool UE4FTFloatLessThanUInt16( float F, const FAUInt16& UI16 )
	{
		return F < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < flt" )
	static bool FUInt16LessThanFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value < F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt < int8" )
	static bool UE4FTFloatLessThanInt8( float F, const FAInt8& I8 )
	{
		return F < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < flt" )
	static bool FInt8LessThanFloat( const FAInt8& I8, float F )
	{
		return I8.Value < F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < int64" )
	static bool FInt64LessThanInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value < I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < uint64" )
	static bool FInt64LessThanUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		if ( ( UI64 >> 63 ) > 0 )
			return true;

		return I64.Value < static_cast< int64 >( UI64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < int64" )
	static bool UE4FTUInt64LessThanInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		if ( ( UI64 >> 63 ) > 0 )
			return false;

		return static_cast< int64 >( UI64.Value ) < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < int32" )
	static bool FInt64LessThanInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value < I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < int64" )
	static bool UE4FTInt32LessThanInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < uint32" )
	static bool FInt64LessThanUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value < UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < int64" )
	static bool FUInt32LessThanInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < int16" )
	static bool FInt64LessThanInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < int64" )
	static bool FInt16LessThanInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < uint16" )
	static bool FInt64LessThanUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < int64" )
	static bool FUInt16LessThanInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < int8" )
	static bool FInt64LessThanInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < int64" )
	static bool FInt8LessThanInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value < I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 < uint8" )
	static bool FInt64LessThanUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < int64" )
	static bool UE4FTUInt8LessThanInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 < I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < uint64" )
	static bool UE4FTUInt64LessThanUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value < UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < int32" )
	static bool UE4FTUInt64LessThanInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value < I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < uint64" )
	static bool UE4FTUInt32LessThanInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < uint32" )
	static bool UE4FTUInt64LessThanUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value < UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < uint64" )
	static bool UE4FTUUInt32LessThanInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < int16" )
	static bool UE4FTUInt64LessThanInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < uint64" )
	static bool FInt16LessThanUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < uint16" )
	static bool UE4FTUInt64LessThanUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < uint64" )
	static bool FUInt16LessThanUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < int8" )
	static bool UE4FTUInt64LessThanInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < uint64" )
	static bool FInt8LessThanUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value < UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 < uint8" )
	static bool UE4FTUInt64LessThanUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < uint64" )
	static bool UE4FTUInt8LessThanUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 < UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < uint32" )
	static bool UE4FTInt32LessThanUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 < UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < int32" )
	static bool FUInt32LessThanInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32 < I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < int16" )
	static bool UE4FTInt32LessThanInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < int32" )
	static bool FInt16LessThanInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value < I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < uint16" )
	static bool UE4FTInt32LessThanUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < int32" )
	static bool FUInt16LessThanInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value < I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 < int8" )
	static bool UE4FTInt32LessThanInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < int32" )
	static bool FInt8LessThanInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value < I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < uint32" )
	static bool FUInt32LessThanUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value < UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < int16" )
	static bool FUInt32LessThanInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32 < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < uint32" )
	static bool FInt16LessThanUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value < UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < uint16" )
	static bool FUInt32LessThanUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < uint32" )
	static bool FUInt16LessThanUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value < UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < int8" )
	static bool FUInt32LessThanInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32 < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < uint32" )
	static bool FInt8LessThanUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value < UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 < uint8" )
	static bool FUInt32LessThanUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < uint32" )
	static bool UE4FTUInt8LessThanUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 < UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < int16" )
	static bool FInt16LessThanInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value < I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < uint16" )
	static bool FInt16LessThanUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < int16" )
	static bool FUInt16LessThanInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < int8" )
	static bool FInt16LessThanInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < int16" )
	static bool FInt8LessThanInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value < I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 < uint8" )
	static bool FInt16LessThanUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < int16" )
	static bool UE4FTUInt8LessThanInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 < I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < uint16" )
	static bool FUInt16LessThanUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value < UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < int8" )
	static bool FUInt16LessThanInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value < I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < uint16" )
	static bool FInt8LessThanUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value < UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 < uint8" )
	static bool FUInt16LessThanUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < uint16" )
	static bool UE4FTUInt8LessThanUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 < UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < int8" )
	static bool FInt8LessThanInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value < I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 < uint8" )
	static bool FInt8LessThanUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value < UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 < int8" )
	static bool UE4FTUInt8LessThanInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 < I8.Value;
	}


	/************************/
	/* GreaterThanOrEqualTo */
	/************************/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= dbl" )
	static bool UE4FTDoubleGreaterThanOrEqualToDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value >= D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= flt" )
	static bool UE4FTDoubleGreaterThanOrEqualToFloat( const FADouble& D, float F )
	{
		return D.Value >= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= dbl" )
	static bool UE4FTFloatGreaterThanOrEqualToDouble( float F, const FADouble& D )
	{
		return F >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= int64" )
	static bool UE4FTDoubleGreaterThanOrEqualToInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= dbl" )
	static bool FInt64GreaterThanOrEqualToDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= uint64" )
	static bool UE4FTDoubleGreaterThanOrEqualToUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= dbl" )
	static bool UE4FTUInt64GreaterThanOrEqualToDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= int32" )
	static bool UE4FTDoubleGreaterThanOrEqualToInt32( const FADouble& D, int32 I32 )
	{
		return D.Value >= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= dbl" )
	static bool UE4FTInt32GreaterThanOrEqualToDouble( int32 I32, const FADouble& D )
	{
		return I32 >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= uint32" )
	static bool UE4FTDoubleGreaterThanOrEqualToUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value >= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= dbl" )
	static bool FUInt32GreaterThanOrEqualToDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= int16" )
	static bool UE4FTDoubleGreaterThanOrEqualToInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= dbl" )
	static bool FInt16GreaterThanOrEqualToDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= uint16" )
	static bool UE4FTDoubleGreaterThanOrEqualToUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= dbl" )
	static bool FUInt16GreaterThanOrEqualToDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= int8" )
	static bool UE4FTDoubleGreaterThanOrEqualToInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= dbl" )
	static bool FInt8GreaterThanOrEqualToDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value >= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl >= uint8" )
	static bool UE4FTDoubleGreaterThanOrEqualToUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= dbl" )
	static bool UE4FTUInt8GreaterThanOrEqualToDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 >= D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= int64" )
	static bool UE4FTFloatGreaterThanOrEqualToInt64( float F, const FAInt64& I64 )
	{
		return F >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= flt" )
	static bool FInt64GreaterThanOrEqualToFloat( const FAInt64& I64, float F )
	{
		return I64.Value >= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= uint64" )
	static bool UE4FTFloatGreaterThanOrEqualToUInt64( float F, const FAUInt64& UI64 )
	{
		return F >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= flt" )
	static bool UE4FTUInt64GreaterThanOrEqualToFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value >= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= uint32" )
	static bool UE4FTFloatGreaterThanOrEqualToUInt32( float F, const FAUInt32& UI32 )
	{
		return F >= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= flt" )
	static bool FUInt32GreaterThanOrEqualToFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value >= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= int16" )
	static bool UE4FTFloatGreaterThanOrEqualToInt16( float F, const FAInt16& I16 )
	{
		return F >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= flt" )
	static bool FInt16GreaterThanOrEqualToFloat( const FAInt16& I16, float F )
	{
		return I16.Value >= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= uint16" )
	static bool UE4FTFloatGreaterThanOrEqualToUInt16( float F, const FAUInt16& UI16 )
	{
		return F >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= flt" )
	static bool FUInt16GreaterThanOrEqualToFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value >= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt >= int8" )
	static bool UE4FTFloatGreaterThanOrEqualToInt8( float F, const FAInt8& I8 )
	{
		return F >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= flt" )
	static bool FInt8GreaterThanOrEqualToFloat( const FAInt8& I8, float F )
	{
		return I8.Value >= F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= int64" )
	static bool FInt64GreaterThanOrEqualToInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value >= I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= uint64" )
	static bool FInt64GreaterThanOrEqualToUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		if ( I64 < 0 )
			return false;

		return static_cast< uint64 >( I64.Value ) >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= int64" )
	static bool UE4FTUInt64GreaterThanOrEqualToInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		if ( I64 < 0 )
			return true;

		return UI64.Value >= static_cast< uint64 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= int32" )
	static bool FInt64GreaterThanOrEqualToInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value >= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= int64" )
	static bool UE4FTInt32GreaterThanOrEqualToInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= uint32" )
	static bool FInt64GreaterThanOrEqualToUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value >= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= int64" )
	static bool FUInt32GreaterThanOrEqualToInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= int16" )
	static bool FInt64GreaterThanOrEqualToInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= int64" )
	static bool FInt16GreaterThanOrEqualToInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= uint16" )
	static bool FInt64GreaterThanOrEqualToUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= int64" )
	static bool FUInt16GreaterThanOrEqualToInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= int8" )
	static bool FInt64GreaterThanOrEqualToInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= int64" )
	static bool FInt8GreaterThanOrEqualToInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value >= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 >= uint8" )
	static bool FInt64GreaterThanOrEqualToUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= int64" )
	static bool UE4FTUInt8GreaterThanOrEqualToInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 >= I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= uint64" )
	static bool UE4FTUInt64GreaterThanOrEqualToUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value >= UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= int32" )
	static bool UE4FTUInt64GreaterThanOrEqualToInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value >= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= uint64" )
	static bool UE4FTUInt32GreaterThanOrEqualToInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= uint32" )
	static bool UE4FTUInt64GreaterThanOrEqualToUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value >= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= uint64" )
	static bool UE4FTUUInt32GreaterThanOrEqualToInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= int16" )
	static bool UE4FTUInt64GreaterThanOrEqualToInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= uint64" )
	static bool FInt16GreaterThanOrEqualToUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= uint16" )
	static bool UE4FTUInt64GreaterThanOrEqualToUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= uint64" )
	static bool FUInt16GreaterThanOrEqualToUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= int8" )
	static bool UE4FTUInt64GreaterThanOrEqualToInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= uint64" )
	static bool FInt8GreaterThanOrEqualToUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value >= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 >= uint8" )
	static bool UE4FTUInt64GreaterThanOrEqualToUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= uint64" )
	static bool UE4FTUInt8GreaterThanOrEqualToUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 >= UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= uint32" )
	static bool UE4FTInt32GreaterThanOrEqualToUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 >= UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= int32" )
	static bool FUInt32GreaterThanOrEqualToInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32 >= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= int16" )
	static bool UE4FTInt32GreaterThanOrEqualToInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= int32" )
	static bool FInt16GreaterThanOrEqualToInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value >= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= uint16" )
	static bool UE4FTInt32GreaterThanOrEqualToUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= int32" )
	static bool FUInt16GreaterThanOrEqualToInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value >= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 >= int8" )
	static bool UE4FTInt32GreaterThanOrEqualToInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= int32" )
	static bool FInt8GreaterThanOrEqualToInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value >= I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= uint32" )
	static bool FUInt32GreaterThanOrEqualToUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value >= UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= int16" )
	static bool FUInt32GreaterThanOrEqualToInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32 >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= uint32" )
	static bool FInt16GreaterThanOrEqualToUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value >= UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= uint16" )
	static bool FUInt32GreaterThanOrEqualToUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= uint32" )
	static bool FUInt16GreaterThanOrEqualToUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value >= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= int8" )
	static bool FUInt32GreaterThanOrEqualToInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32 >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= uint32" )
	static bool FInt8GreaterThanOrEqualToUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value >= UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 >= uint8" )
	static bool FUInt32GreaterThanOrEqualToUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= uint32" )
	static bool UE4FTUInt8GreaterThanOrEqualToUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 >= UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= int16" )
	static bool FInt16GreaterThanOrEqualToInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value >= I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= uint16" )
	static bool FInt16GreaterThanOrEqualToUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= int16" )
	static bool FUInt16GreaterThanOrEqualToInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= int8" )
	static bool FInt16GreaterThanOrEqualToInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= int16" )
	static bool FInt8GreaterThanOrEqualToInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value >= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 >= uint8" )
	static bool FInt16GreaterThanOrEqualToUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= int16" )
	static bool UE4FTUInt8GreaterThanOrEqualToInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 >= I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= uint16" )
	static bool FUInt16GreaterThanOrEqualToUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value >= UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= int8" )
	static bool FUInt16GreaterThanOrEqualToInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value >= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= uint16" )
	static bool FInt8GreaterThanOrEqualToUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value >= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 >= uint8" )
	static bool FUInt16GreaterThanOrEqualToUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= uint16" )
	static bool UE4FTUInt8GreaterThanOrEqualToUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 >= UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= int8" )
	static bool FInt8GreaterThanOrEqualToInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value >= I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 >= uint8" )
	static bool FInt8GreaterThanOrEqualToUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value >= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 >= int8" )
	static bool UE4FTUInt8GreaterThanOrEqualToInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 >= I8.Value;
	}


	/*********************/
	/* LessThanOrEqualTo */
	/*********************/

	/* Double */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= dbl" )
	static bool UE4FTDoubleLessThanOrEqualToDouble( const FADouble& D1, const FADouble& D2 )
	{
		return D1.Value <= D2.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= flt" )
	static bool UE4FTDoubleLessThanOrEqualToFloat( const FADouble& D, float F )
	{
		return D.Value <= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= dbl" )
	static bool UE4FTFloatLessThanOrEqualToDouble( float F, const FADouble& D )
	{
		return F <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= int64" )
	static bool UE4FTDoubleLessThanOrEqualToInt64( const FADouble& D, const FAInt64& I64 )
	{
		return D.Value <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= dbl" )
	static bool FInt64LessThanOrEqualToDouble( const FAInt64& I64, const FADouble& D )
	{
		return I64.Value <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= uint64" )
	static bool UE4FTDoubleLessThanOrEqualToUInt64( const FADouble& D, const FAUInt64& UI64 )
	{
		return D.Value <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= dbl" )
	static bool UE4FTUInt64LessThanOrEqualToDouble( const FAUInt64& UI64, const FADouble& D )
	{
		return UI64.Value <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= int32" )
	static bool UE4FTDoubleLessThanOrEqualToInt32( const FADouble& D, int32 I32 )
	{
		return D.Value <= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= dbl" )
	static bool UE4FTInt32LessThanOrEqualToDouble( int32 I32, const FADouble& D )
	{
		return I32 <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= uint32" )
	static bool UE4FTDoubleLessThanOrEqualToUInt32( const FADouble& D, const FAUInt32& UI32 )
	{
		return D.Value <= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= dbl" )
	static bool FUInt32LessThanOrEqualToDouble( const FAUInt32& UI32, const FADouble& D )
	{
		return UI32.Value <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= int16" )
	static bool UE4FTDoubleLessThanOrEqualToInt16( const FADouble& D, const FAInt16& I16 )
	{
		return D.Value <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= dbl" )
	static bool FInt16LessThanOrEqualToDouble( const FAInt16& I16, const FADouble& D )
	{
		return I16.Value <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= uint16" )
	static bool UE4FTDoubleLessThanOrEqualToUInt16( const FADouble& D, const FAUInt16& UI16 )
	{
		return D.Value <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= dbl" )
	static bool FUInt16LessThanOrEqualToDouble( const FAUInt16& UI16, const FADouble& D )
	{
		return UI16.Value <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= int8" )
	static bool UE4FTDoubleLessThanOrEqualToInt8( const FADouble& D, const FAInt8& I8 )
	{
		return D.Value <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= dbl" )
	static bool FInt8LessThanOrEqualToDouble( const FAInt8& I8, const FADouble& D )
	{
		return I8.Value <= D.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "dbl <= uint8" )
	static bool UE4FTDoubleLessThanOrEqualToUInt8( const FADouble& D, uint8 UI8 )
	{
		return D.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= dbl" )
	static bool UE4FTUInt8LessThanOrEqualToDouble( uint8 UI8, const FADouble& D )
	{
		return UI8 <= D.Value;
	}

	/* Float */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= int64" )
	static bool UE4FTFloatLessThanOrEqualToInt64( float F, const FAInt64& I64 )
	{
		return F <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= flt" )
	static bool FInt64LessThanOrEqualToFloat( const FAInt64& I64, float F )
	{
		return I64.Value <= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= uint64" )
	static bool UE4FTFloatLessThanOrEqualToUInt64( float F, const FAUInt64& UI64 )
	{
		return F <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= flt" )
	static bool UE4FTUInt64LessThanOrEqualToFloat( const FAUInt64& UI64, float F )
	{
		return UI64.Value <= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= uint32" )
	static bool UE4FTFloatLessThanOrEqualToUInt32( float F, const FAUInt32& UI32 )
	{
		return F <= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= flt" )
	static bool FUInt32LessThanOrEqualToFloat( const FAUInt32& UI32, float F )
	{
		return UI32.Value <= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= int16" )
	static bool UE4FTFloatLessThanOrEqualToInt16( float F, const FAInt16& I16 )
	{
		return F <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= flt" )
	static bool FInt16LessThanOrEqualToFloat( const FAInt16& I16, float F )
	{
		return I16.Value <= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= uint16" )
	static bool UE4FTFloatLessThanOrEqualToUInt16( float F, const FAUInt16& UI16 )
	{
		return F <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= flt" )
	static bool FUInt16LessThanOrEqualToFloat( const FAUInt16& UI16, float F )
	{
		return UI16.Value <= F;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "flt <= int8" )
	static bool UE4FTFloatLessThanOrEqualToInt8( float F, const FAInt8& I8 )
	{
		return F <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= flt" )
	static bool FInt8LessThanOrEqualToFloat( const FAInt8& I8, float F )
	{
		return I8.Value <= F;
	}

	/* Int 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= int64" )
	static bool FInt64LessThanOrEqualToInt64( const FAInt64& I641, const FAInt64& I642 )
	{
		return I641.Value <= I642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= uint64" )
	static bool FInt64LessThanOrEqualToUInt64( const FAInt64& I64, const FAUInt64& UI64 )
	{
		if ( I64 < 0 )
			return true;

		return static_cast< uint64 >( I64.Value ) <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= int64" )
	static bool UE4FTUInt64LessThanOrEqualToInt64( const FAUInt64& UI64, const FAInt64& I64 )
	{
		if ( I64 < 0 )
			return false;

		return UI64.Value <= static_cast< uint64 >( I64.Value );
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= int32" )
	static bool FInt64LessThanOrEqualToInt32( const FAInt64& I64, int32 I32 )
	{
		return I64.Value <= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= int64" )
	static bool UE4FTInt32LessThanOrEqualToInt64( int32 I32, const FAInt64& I64 )
	{
		return I32 <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= uint32" )
	static bool FInt64LessThanOrEqualToUInt32( const FAInt64& I64, const FAUInt32& UI32 )
	{
		return I64.Value <= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= int64" )
	static bool FUInt32LessThanOrEqualToInt64( const FAUInt32& UI32, const FAInt64& I64 )
	{
		return UI32.Value <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= int16" )
	static bool FInt64LessThanOrEqualToInt16( const FAInt64& I64, const FAInt16& I16 )
	{
		return I64.Value <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= int64" )
	static bool FInt16LessThanOrEqualToInt64( const FAInt16& I16, const FAInt64& I64 )
	{
		return I16.Value <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= uint16" )
	static bool FInt64LessThanOrEqualToUInt16( const FAInt64& I64, const FAUInt16& UI16 )
	{
		return I64.Value <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= int64" )
	static bool FUInt16LessThanOrEqualToInt64( const FAUInt16& UI16, const FAInt64& I64 )
	{
		return UI16.Value <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= int8" )
	static bool FInt64LessThanOrEqualToInt8( const FAInt64& I64, const FAInt8& I8 )
	{
		return I64.Value <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= int64" )
	static bool FInt8LessThanOrEqualToInt64( const FAInt8& I8, const FAInt64& I64 )
	{
		return I8.Value <= I64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int64 <= uint8" )
	static bool FInt64LessThanOrEqualToUInt8( const FAInt64& I64, uint8 UI8 )
	{
		return I64.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= int64" )
	static bool UE4FTUInt8LessThanOrEqualToInt64( uint8 UI8, const FAInt64& I64 )
	{
		return UI8 <= I64.Value;
	}

	/* UInt 64 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= uint64" )
	static bool UE4FTUInt64LessThanOrEqualToUInt64( const FAUInt64& UI641, const FAUInt64& UI642 )
	{
		return UI641.Value <= UI642.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= int32" )
	static bool UE4FTUInt64LessThanOrEqualToInt32( const FAUInt64& UI64, int32 I32 )
	{
		return UI64.Value <= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= uint64" )
	static bool UE4FTUInt32LessThanOrEqualToInt64( int32 I32, const FAUInt64& UI64 )
	{
		return I32 <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= uint32" )
	static bool UE4FTUInt64LessThanOrEqualToUInt32( const FAUInt64& UI64, const FAUInt32& UI32 )
	{
		return UI64.Value <= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= uint64" )
	static bool UE4FTUUInt32LessThanOrEqualToInt64( const FAUInt32& UI32, const FAUInt64& UI64 )
	{
		return UI32.Value <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= int16" )
	static bool UE4FTUInt64LessThanOrEqualToInt16( const FAUInt64& UI64, const FAInt16& I16 )
	{
		return UI64.Value <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= uint64" )
	static bool FInt16LessThanOrEqualToUInt64( const FAInt16& I16, const FAUInt64& UI64 )
	{
		return I16.Value <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= uint16" )
	static bool UE4FTUInt64LessThanOrEqualToUInt16( const FAUInt64& UI64, const FAUInt16& UI16 )
	{
		return UI64.Value <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= uint64" )
	static bool FUInt16LessThanOrEqualToUInt64( const FAUInt16& UI16, const FAUInt64& UI64 )
	{
		return UI16.Value <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= int8" )
	static bool UE4FTUInt64LessThanOrEqualToInt8( const FAUInt64& UI64, const FAInt8& I8 )
	{
		return UI64.Value <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= uint64" )
	static bool FInt8LessThanOrEqualToUInt64( const FAInt8& I8, const FAUInt64& UI64 )
	{
		return I8.Value <= UI64.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint64 <= uint8" )
	static bool UE4FTUInt64LessThanOrEqualToUInt8( const FAUInt64& UI64, uint8 UI8 )
	{
		return UI64.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= uint64" )
	static bool UE4FTUInt8LessThanOrEqualToUInt64( uint8 UI8, const FAUInt64& UI64 )
	{
		return UI8 <= UI64.Value;
	}

	/* Int 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= uint32" )
	static bool UE4FTInt32LessThanOrEqualToUInt32( int32 I32, const FAUInt32& UI32 )
	{
		return I32 <= UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= int32" )
	static bool FUInt32LessThanOrEqualToInt32( const FAUInt32& UI32, int32 I32 )
	{
		return UI32 <= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= int16" )
	static bool UE4FTInt32LessThanOrEqualToInt16( int32 I32, const FAInt16& I16 )
	{
		return I32 <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= int32" )
	static bool FInt16LessThanOrEqualToInt32( const FAInt16& I16, int32 I32 )
	{
		return I16.Value <= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= uint16" )
	static bool UE4FTInt32LessThanOrEqualToUInt16( int32 I32, const FAUInt16& UI16 )
	{
		return I32 <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= int32" )
	static bool FUInt16LessThanOrEqualToInt32( const FAUInt16& UI16, int32 I32 )
	{
		return UI16.Value <= I32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int32 <= int8" )
	static bool UE4FTInt32LessThanOrEqualToInt8( int32 I32, const FAInt8& I8 )
	{
		return I32 <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= int32" )
	static bool FInt8LessThanOrEqualToInt32( const FAInt8& I8, int32 I32 )
	{
		return I8.Value <= I32;
	}

	/* UInt 32 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= uint32" )
	static bool FUInt32LessThanOrEqualToUInt32( const FAUInt32& UI321, const FAUInt32& UI322 )
	{
		return UI321.Value <= UI322.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= int16" )
	static bool FUInt32LessThanOrEqualToInt16( const FAUInt32& UI32, const FAInt16& I16 )
	{
		return UI32 <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= uint32" )
	static bool FInt16LessThanOrEqualToUInt32( const FAInt16& I16, const FAUInt32& UI32 )
	{
		return I16.Value <= UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= uint16" )
	static bool FUInt32LessThanOrEqualToUInt16( const FAUInt32& UI32, const FAUInt16& UI16 )
	{
		return UI32.Value <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= uint32" )
	static bool FUInt16LessThanOrEqualToUInt32( const FAUInt16& UI16, const FAUInt32& UI32 )
	{
		return UI16.Value <= UI32.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= int8" )
	static bool FUInt32LessThanOrEqualToInt8( const FAUInt32& UI32, const FAInt8& I8 )
	{
		return UI32 <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= uint32" )
	static bool FInt8LessThanOrEqualToUInt32( const FAInt8& I8, const FAUInt32& UI32 )
	{
		return I8.Value <= UI32;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint32 <= uint8" )
	static bool FUInt32LessThanOrEqualToUInt8( const FAUInt32& UI32, uint8 UI8 )
	{
		return UI32.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= uint32" )
	static bool UE4FTUInt8LessThanOrEqualToUInt32( uint8 UI8, const FAUInt32& UI32 )
	{
		return UI8 <= UI32.Value;
	}

	/* Int 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= int16" )
	static bool FInt16LessThanOrEqualToInt16( const FAInt16& I161, const FAInt16& I162 )
	{
		return I161.Value <= I162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= uint16" )
	static bool FInt16LessThanOrEqualToUInt16( const FAInt16& I16, const FAUInt16& UI16 )
	{
		return I16.Value <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= int16" )
	static bool FUInt16LessThanOrEqualToInt16( const FAUInt16& UI16, const FAInt16& I16 )
	{
		return UI16.Value <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= int8" )
	static bool FInt16LessThanOrEqualToInt8( const FAInt16& I16, const FAInt8& I8 )
	{
		return I16.Value <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= int16" )
	static bool FInt8LessThanOrEqualToInt16( const FAInt8& I8, const FAInt16& I16 )
	{
		return I8.Value <= I16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int16 <= uint8" )
	static bool FInt16LessThanOrEqualToUInt8( const FAInt16& I16, uint8 UI8 )
	{
		return I16.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= int16" )
	static bool UE4FTUInt8LessThanOrEqualToInt16( uint8 UI8, const FAInt16& I16 )
	{
		return UI8 <= I16.Value;
	}

	/* UInt 16 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= uint16" )
	static bool FUInt16LessThanOrEqualToUUInt16( const FAUInt16& UI161, const FAUInt16& UI162 )
	{
		return UI161.Value <= UI162.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= int8" )
	static bool FUInt16LessThanOrEqualToInt8( const FAUInt16& UI16, const FAInt8& I8 )
	{
		return UI16.Value <= I8.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= uint16" )
	static bool FInt8LessThanOrEqualToUInt16( const FAInt8& I8, const FAUInt16& UI16 )
	{
		return I8.Value <= UI16.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint16 <= uint8" )
	static bool FUInt16LessThanOrEqualToUInt8( const FAUInt16& UI16, uint8 UI8 )
	{
		return UI16.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= uint16" )
	static bool UE4FTUInt8LessThanOrEqualToUInt16( uint8 UI8, const FAUInt16& UI16 )
	{
		return UI8 <= UI16.Value;
	}

	/* Int 8 */

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= int8" )
	static bool FInt8LessThanOrEqualToInt8( const FAInt8& I81, const FAInt8& I82 )
	{
		return I81.Value <= I82.Value;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "int8 <= uint8" )
	static bool FInt8LessThanOrEqualToUInt8( const FAInt8& I8, uint8 UI8 )
	{
		return I8.Value <= UI8;
	}

	UFUNCTION( Category = "NetPeer|UE4 Fundamental Types", BlueprintPure, DisplayName = "uint8 <= int8" )
	static bool UE4FTUInt8LessThanOrEqualToInt8( uint8 UI8, const FAInt8& I8 )
	{
		return UI8 <= I8.Value;
	}

};
