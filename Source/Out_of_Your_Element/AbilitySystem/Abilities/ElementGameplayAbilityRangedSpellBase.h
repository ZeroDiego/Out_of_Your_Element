// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElementGameplayAbilitySpellBase.h"
#include "ElementGameplayAbilityRangedSpellBase.generated.h"

class AElementProjectileBase;
class UNiagaraSystem;

UCLASS(Abstract)
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbilityRangedSpellBase : public UElementGameplayAbilitySpellBase
{
	GENERATED_BODY()

public:
	static inline FName GetNoPlaceTag();

	static bool TraceSpell(const AActor* Caster, OUT FHitResult& OutHitResult);

	static bool CanPlace(const FHitResult& HitResult);

	static bool GetSpellLocation(const AActor* Caster, OUT FVector& OutLocation);

protected:
	virtual void CastSpell(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void CastSpellAtLocation(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData,
		const FVector& Location
	) PURE_VIRTUAL(UElementGameplayAbilityRangedSpellBase::CastSpellAtLocation);
};
