// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElementGameplayAbilitySpellBase.h"
#include "Out_of_Your_Element/Projectile/ElementZoneBase.h"
#include "ElementGameplayAbility_Meteor.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Meteor : public UElementGameplayAbilitySpellBase
{
	GENERATED_BODY()

public:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	FVector MeteorSpawnOffset = FVector(0, 0, 5000);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	TSubclassOf<class AElementMeteor> MeteorClass;
};
