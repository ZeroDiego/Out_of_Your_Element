// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementGameplayAbilitySpellBase.h"
#include "Abilities/GameplayAbility.h"
#include "Out_of_Your_Element/Projectile/ElementWallBase.h"
#include "ElementGameplayAbility_RockWall.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_RockWall : public UElementGameplayAbilitySpellBase
{
	GENERATED_BODY()

public:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AElementWallBase> ElementWallBase;
};
