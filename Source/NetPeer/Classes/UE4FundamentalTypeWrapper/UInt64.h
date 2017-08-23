// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "UInt64.generated.h"

USTRUCT( BlueprintType )
struct FAUInt64 : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 Value;

	operator uint64() const { return Value; }

	FAUInt64() { this->Value = 0; }
	FAUInt64( uint64 _Value ) { this->Value = _Value; }

};
