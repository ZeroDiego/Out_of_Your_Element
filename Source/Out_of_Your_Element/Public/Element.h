#pragma once

#include "GameplayTagContainer.h"
#include "Element.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag DamageType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> BaseAttackAbility;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> HeavyAttackAbility;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> SpecialAttackAbility;
};
