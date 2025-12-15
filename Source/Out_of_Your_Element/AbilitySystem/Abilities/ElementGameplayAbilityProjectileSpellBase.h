// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "Abilities/GameplayAbility.h"
#include "ElementGameplayAbilitySpellBase.h"
#include "ElementGameplayAbilityProjectileSpellBase.generated.h"

class AElementProjectileBase;
class UNiagaraSystem;

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbilityProjectileSpellBase : public UElementGameplayAbilitySpellBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Element Ability Projectile")
	TSubclassOf<AElementProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Centimeters"), Category="Element Ability Projectile")
	float ProjectileSpawnOffset = 50.0f;

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	void ShootProjectile(const FTransform& Transform, int Level) const;
};
