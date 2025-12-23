// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_Meteor.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/Projectile/ElementMeteor.h"

void UElementGameplayAbility_Meteor::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData
	);

	Super::CastSpell(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData
	);
}

void UElementGameplayAbility_Meteor::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
}

void UElementGameplayAbility_Meteor::CastSpellAtLocation(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData,
	const FVector& Location
)
{
	if (!ActorInfo->AvatarActor.IsValid())
		return;

	AActor* Caster = ActorInfo->AvatarActor.Get();

	const FVector SpawnDirection = Caster->GetActorForwardVector();
	FVector SpawnLocation = Location + MeteorSpawnOffset;

	const int Level = GetAbilityLevel(Handle, ActorInfo);
	if (const int SpawnCount = Level < 2 ? 1 : MeteorCount; SpawnCount > 1)
	{
		FTimerManager& TimerManager = Caster->GetWorld()->GetTimerManager();
		for (int i = 0; i < SpawnCount; ++i)
		{
			if (const float Delay = MeteorSpawnDelay * i; Delay > 0)
			{
				FTimerHandle TimerHandle;
				TimerManager.SetTimer(
					TimerHandle,
					FTimerDelegate::CreateWeakLambda(this, [=, this]
					{
						SpawnMeteor(Caster, SpawnLocation, Level);

						if (i == SpawnCount - 1)
							EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
					}),
					Delay,
					false
				);
			}
			else
			{
				SpawnMeteor(Caster, SpawnLocation, Level);
			}

			SpawnLocation += MeteorSpacing * SpawnDirection;
		}
	}
	else
	{
		SpawnMeteor(Caster, SpawnLocation, Level);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UElementGameplayAbility_Meteor::EndSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
}

void UElementGameplayAbility_Meteor::SpawnMeteor(
	AActor* Caster,
	const FVector& SpawnLocation,
	const int Level
) const
{
	const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
	if (AElementMeteor* Meteor = Caster->GetWorld()->SpawnActorDeferred<AElementMeteor>(
		MeteorClass,
		SpawnTransform
	))
	{
		Meteor->Caster = Caster;
		Meteor->Level = Level;

		if (Level >= 2)
		{
			Meteor->ImpactDamage *= MeteorImpactMultiplier;
		}

		UGameplayStatics::FinishSpawningActor(Meteor, SpawnTransform);
	}
}
