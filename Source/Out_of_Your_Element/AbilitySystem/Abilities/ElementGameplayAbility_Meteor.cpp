// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_Meteor.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Projectile/ElementMeteor.h"

void UElementGameplayAbility_Meteor::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (PlayerController->IsLocalPlayerController())
			{
				static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
					UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2),
				};

				if (FHitResult MouseCursorHitResult; PlayerController->GetHitResultUnderCursorForObjects(
					GroundTypes, false, MouseCursorHitResult))
				{
					MeteorSpawnLocation = FTransform(FRotator::ZeroRotator, MouseCursorHitResult.Location);

					const FTransform MeteorProjectileSpawnLocation = FTransform(
						FRotator::ZeroRotator, MouseCursorHitResult.Location + MeteorSpawnOffset
					);

					if (AElementMeteor* Meteor = GetWorld()->SpawnActorDeferred<AElementMeteor>(
						MeteorClass,
						MeteorProjectileSpawnLocation
					))
					{
						Meteor->SummoningTime = MeteorSummoningTime;
						Meteor->TargetLocation = MouseCursorHitResult.Location;

						UGameplayStatics::FinishSpawningActor(Meteor, MeteorProjectileSpawnLocation);
					}

					UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, MeteorSummoningTime);
					DelayTask->OnFinish.AddDynamic(this, &UElementGameplayAbility_Meteor::OnDelayFinished);
					DelayTask->ReadyForActivation();
				}
			}
		}
	}
}

void UElementGameplayAbility_Meteor::OnDelayFinished()
{
	if (AElementZoneBase* MeteorZone = GetWorld()->SpawnActorDeferred<AElementZoneBase>(
		MeteorZoneClass,
		MeteorSpawnLocation,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		const FGameplayEffectSpecHandle DotGameplayEffectSpecHandle =
			MakeOutgoingGameplayEffectSpec(DamageGameplayEffect);

		DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Duration,
			DotDamageDuration
		);

		DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Damage,
			BaseDamage
		);

		MeteorZone->InitializeZone(
			DotGameplayEffectSpecHandle,
			this,
			MeteorZoneVfx,
			MeteorZoneRadius,
			MeteorZoneLifeSpan
		);

		UGameplayStatics::FinishSpawningActor(MeteorZone, MeteorSpawnLocation);

		const FGameplayEffectSpecHandle ImpactGameplayEffectSpecHandle =
			MakeOutgoingGameplayEffectSpec(ImpactDamageGameplayEffect);

		ImpactGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
			ElementGameplayTags::Abilities_Parameters_Damage,
			ImpactBaseDamage
		);

		TArray<AActor*> HitActors;
		MeteorZone->GetOverlappingActors(HitActors, AElementCharacterBase::StaticClass());
		for (AActor* HitActor : HitActors)
		{
			if (const AElementCharacterBase* HitCharacter = Cast<AElementCharacterBase>(HitActor))
			{
				HitCharacter->ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(
					ImpactGameplayEffectSpecHandle
				);
			}
		}
	}

	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
