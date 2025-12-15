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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	FVector MeteorSpawnOffset = FVector(0, 0, 5000);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor")
	TSubclassOf<class AElementMeteor> MeteorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor Level 2")
	int MeteorImpactMultiplier = 0.75f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Meteor Level 2")
	int MeteorCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Unit="Centimeter"), Category="Meteor Level 2")
	float MeteorSpacing = 500.0f;

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	void SpawnMeteor(
		AActor* Caster,
		const FVector& SpawnLocation,
		int Level = 1
	) const;
};
