// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementGameplayAbilityBase.h"
#include "Out_of_Your_Element/Projectile/ElementZoneBase.h"
#include "ElementGameplayAbility_Meteor.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Meteor : public UElementGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
								 const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AElementZoneBase> MeteorZoneClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Meteor")
	float MeteorSummoningTime = 3;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Meteor")
	float DotDamageDuration = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireZone")
	float MeteorZoneRadius = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Meteor")
	float MeteorZoneLifeSpan = 10;

	UPROPERTY(EditDefaultsOnly, Category="VFX")
	UNiagaraSystem* MeteorZoneVfx;

protected:
	UFUNCTION()
	void OnDelayFinished();

private:
	FTransform MeteorSpawnLocation;
};
