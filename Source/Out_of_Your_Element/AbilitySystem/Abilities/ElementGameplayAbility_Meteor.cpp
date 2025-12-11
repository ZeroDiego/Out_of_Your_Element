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

	if (const ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (PlayerController->IsLocalPlayerController())
			{
				if (FHitResult MouseCursorHitResult;
					PlayerController->GetHitResultUnderCursorByChannel(
						UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
						false,
						MouseCursorHitResult
					)
				)
				{
					const FVector TargetLocation = MouseCursorHitResult.ImpactPoint;
					const FTransform MeteorProjectileSpawnLocation = FTransform(
						FRotator::ZeroRotator, TargetLocation + MeteorSpawnOffset
					);

					if (AElementMeteor* Meteor = GetWorld()->SpawnActorDeferred<AElementMeteor>(
						MeteorClass,
						MeteorProjectileSpawnLocation
					))
					{
						Meteor->TargetLocation = TargetLocation;
						UGameplayStatics::FinishSpawningActor(Meteor, MeteorProjectileSpawnLocation);
					}
				}
			}
		}
	}
}

void UElementGameplayAbility_Meteor::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
}
