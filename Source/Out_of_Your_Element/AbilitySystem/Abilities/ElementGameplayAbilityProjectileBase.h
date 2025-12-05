// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "Abilities/GameplayAbility.h"
#include "ElementGameplayAbilityBase.h"
#include "ElementGameplayAbilityProjectileBase.generated.h"

class AElementProjectileBase;
class UNiagaraSystem;

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbilityProjectileBase : public UElementGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Element Ability Projectile")
	TSubclassOf<AElementProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Centimeters"), Category="Element Ability Projectile")
	float ProjectileSpawnOffset = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Element Ability Projectile")
	UNiagaraSystem* ProjectileVfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Element Ability Projectile")
	UNiagaraSystem* ProjectileHitVfx;
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
	UFUNCTION()
	virtual void OnDelayFinished();
	
	UFUNCTION()
	virtual void OnAnimationFinished(FGameplayEventData GameplayEventData);

	UFUNCTION()
	virtual void OnAbilityFinished(FGameplayEventData GameplayEventData);

	void ShootProjectile(const FVector& Location, const FRotator& Direction);
	void ShootProjectile(const FTransform& Transform);

	virtual void SetAttackingTrue();
	virtual void SetAttackingFalse();
};
