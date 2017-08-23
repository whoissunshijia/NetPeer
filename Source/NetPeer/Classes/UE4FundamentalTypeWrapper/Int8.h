// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "Int8.generated.h"

USTRUCT( BlueprintType )
struct FAInt8 : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	int8 Value;

	operator int8() const { return Value; }

	FAInt8() { this->Value = 0; }
	FAInt8( int8 _Value ) { this->Value = _Value; }

};
