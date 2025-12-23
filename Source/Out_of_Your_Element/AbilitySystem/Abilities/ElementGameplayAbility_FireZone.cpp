// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementGameplayAbility_FireZone.h"

#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void UElementGameplayAbility_FireZone::CastSpellAtLocation(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData,
	const FVector& Location
)
{
	const FTransform SpawnTransform(FRotator::ZeroRotator, Location);

	if (AElementZoneBase* FireZone = GetWorld()->SpawnActorDeferred<AElementZoneBase>(
		FireZoneClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		const FGameplayEffectSpecHandle FireZoneGameplayEffectSpecHandle =
			MakeOutgoingGameplayEffectSpec(DamageGameplayEffect);

		FireZoneGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Duration,
			FireZoneDamageDuration
		);

		FireZoneGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Damage,
			BaseDamage
		);

		const int Level = GetAbilityLevel(Handle, ActorInfo);
		const float ActualRadius = Level < 2
			                           ? FireZoneRadius
			                           : FireZoneRadius + AdditionalFireZoneRadius;
		FireZone->InitializeZone(
			FireZoneGameplayEffectSpecHandle,
			Level < 2 ? FireZoneVfx : LargeFireZoneVfx,
			ActualRadius,
			FireZoneLifeSpan
		);

		UGameplayStatics::FinishSpawningActor(FireZone, SpawnTransform);
	}
}
