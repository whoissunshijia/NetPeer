// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "Int16.generated.h"

USTRUCT( BlueprintType )
struct FAInt16 : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	int16 Value;

	operator int16() const { return Value; }

	FAInt16() { this->Value = 0; }
	FAInt16( int16 _Value ) { this->Value = _Value; }

};

