// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementGameplayAbilityBase.h"
#include "Abilities/GameplayAbility.h"
#include "Out_of_Your_Element/Projectile/ElementWallBase.h"
#include "ElementGameplayAbility_RockWall.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_RockWall : public UElementGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UElementGameplayAbility_RockWall();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AElementWallBase> ElementWallBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> RockWallDamageGameplayEffect;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* RockWallPopInVfx;

	UPROPERTY(EditAnywhere, Category="VFX")
	UNiagaraSystem* RockWallPopOutVfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RockWallScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RockWallDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RockWallOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RockWallLifeSpan;
};
