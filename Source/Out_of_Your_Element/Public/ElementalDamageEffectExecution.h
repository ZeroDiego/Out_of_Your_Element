#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "ElementalDamageEffectExecution.generated.h"

UCLASS()
class UElementalDamageEffectExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UElementalDamageEffectExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
