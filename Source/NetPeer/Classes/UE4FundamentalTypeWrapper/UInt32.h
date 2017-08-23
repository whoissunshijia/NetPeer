// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "UInt32.generated.h"

USTRUCT( BlueprintType )
struct FAUInt32 : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 Value;

	operator int32() const { return (int32)Value; }

	FAUInt32() { this->Value = 0; }
	FAUInt32( uint32 _Value ) { this->Value = _Value; }

};