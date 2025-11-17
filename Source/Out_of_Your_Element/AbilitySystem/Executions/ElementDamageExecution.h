#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "ElementDamageExecution.generated.h"

UCLASS()
class UElementDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
