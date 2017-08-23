// Copyright 2014-2017 Barry, Inc. All Rights Reserved.

#pragma once
#include "UE4FundamentalTypeBase.h"
#include "Double.generated.h"

USTRUCT( BlueprintType )
struct FADouble : public FRawDataTypeBase
{
	GENERATED_BODY()

	UPROPERTY()
	double Value;

	operator double() const { return Value; }

	FADouble() { this->Value = 0; }
	FADouble( double _Value ) { this->Value = _Value; }

};