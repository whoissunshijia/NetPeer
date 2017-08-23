// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "Int64.generated.h"

USTRUCT( BlueprintType )
struct FAInt64 : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Value;

	operator int64() const { return Value; }

	FAInt64() { this->Value = 0; }
	FAInt64( int64 _Value ) { this->Value = _Value; }

};