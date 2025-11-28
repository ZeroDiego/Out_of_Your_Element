#pragma once

#include "BlueprintUtility.generated.h"

// Blueprint does not like references as inputs. So this hackery is required. See https://dev.epicgames.com/community/learning/tutorials/eK9V/unreal-engine-reference-parameters-in-blueprint-events
USTRUCT(BlueprintType)
struct OUT_OF_YOUR_ELEMENT_API FMutableBool
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool Boolean;

	bool* BooleanPtr = &Boolean;

	FMutableBool() : Boolean(false) {}
	
	// ReSharper disable once CppNonExplicitConvertingConstructor
	FMutableBool(const bool Boolean) : Boolean(Boolean) {}
	
	// ReSharper disable once CppNonExplicitConversionOperator
	operator bool() const { return Boolean; }
};

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UWrapperValuesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void SetBoolean(const FMutableBool& Boolean, const bool Value)
	{
		*const_cast<bool*>(Boolean.BooleanPtr) = Value;
	}
};
