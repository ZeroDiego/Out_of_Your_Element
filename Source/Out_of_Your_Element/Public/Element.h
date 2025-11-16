#pragma once

#include "GameplayTagContainer.h"
#include "Element.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> BaseAttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> HeavyAttackAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> SpecialAttackAbility;
};
