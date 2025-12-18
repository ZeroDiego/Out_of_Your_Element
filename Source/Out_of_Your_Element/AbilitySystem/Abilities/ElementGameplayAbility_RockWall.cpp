// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementGameplayAbility_RockWall.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void UElementGameplayAbility_RockWall::CastSpellAtLocation(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData,
	const FVector& Location
)
{
	if (const AElementCharacterBase* Caster = Cast<AElementCharacterBase>(ActorInfo->AvatarActor))
	{
		const FTransform MouseCursorTransform(
			Caster->GetActorRotation(),
			Location
		);

		if (AElementWallBase* RockWall = GetWorld()->SpawnActorDeferred<AElementWallBase>(
			ElementWallBase,
			MouseCursorTransform,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			const FGameplayEffectSpecHandle RockWallGameplayEffectSpecHandle =
				MakeOutgoingGameplayEffectSpec(
					DamageGameplayEffect,
					1);

			RockWallGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				BaseDamage);

			RockWall->GameplayEffectSpecHandle = RockWallGameplayEffectSpecHandle;
			RockWall->Caster = Caster;
			UGameplayStatics::FinishSpawningActor(RockWall, MouseCursorTransform);
		}
	}
}
