// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElementGameplayAbilitySpellBase.h"
#include "ElementGameplayAbilityRangedSpellBase.generated.h"

class AElementProjectileBase;
class UNiagaraSystem;

static constexpr float MinRangedSpellPlacementRange = 150.0f;

static constexpr float MaxRangedSpellPlacementRange = 750.0f;

USTRUCT()
struct FTargetLocationData : public FGameplayAbilityTargetData
{
	GENERATED_BODY()

public:
	FHitResult HitResult = FHitResult();

	virtual bool HasHitResult() const override;

	virtual const FHitResult* GetHitResult() const override;

	virtual UScriptStruct* GetScriptStruct() const override;
};

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
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;

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

	virtual void CastSpellAtLocation(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData,
		const FVector& Location
	) PURE_VIRTUAL(UElementGameplayAbilityRangedSpellBase::CastSpellAtLocation);
};
