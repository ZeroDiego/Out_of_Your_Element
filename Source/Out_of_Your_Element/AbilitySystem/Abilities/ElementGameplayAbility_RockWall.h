// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementGameplayAbilityRangedSpellBase.h"
#include "Out_of_Your_Element/Projectile/ElementWallBase.h"
#include "ElementGameplayAbility_RockWall.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_RockWall : public UElementGameplayAbilityRangedSpellBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="RockWall")
	TSubclassOf<AElementWallBase> ElementWallBase;

protected:
	virtual void CastSpellAtLocation(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData,
		const FVector& Location
	) override;
};
