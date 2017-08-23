// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "UInt16.generated.h"

USTRUCT( BlueprintType )
struct FAUInt16 : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 Value;

	operator uint16() const { return Value; }

	FAUInt16() { this->Value = 0; }
	FAUInt16( uint16 _Value ) { this->Value = _Value; }

};
