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

	if (ACharacter* Caster = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Caster->GetController()))
		{
			if (PlayerController->IsLocalPlayerController())
			{
				static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
					UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)
				};

				if (FHitResult MouseCursorHitResult;
					PlayerController->GetHitResultUnderCursorForObjects(
						GroundTypes,
						false,
						MouseCursorHitResult
					)
				)
				{
					const FVector TargetLocation = MouseCursorHitResult.ImpactPoint;
					const FVector SpawnDirection = Caster->GetActorForwardVector();
					FVector SpawnLocation = TargetLocation + MeteorSpawnOffset;

					const int Level = GetAbilityLevel(Handle, ActorInfo);
					const int SpawnCount = Level < 2 ? 1 : MeteorCount;

					for (int i = 0; i < SpawnCount; ++i)
					{
						SpawnMeteor(Caster, SpawnLocation, Level);
						SpawnLocation += MeteorSpacing * SpawnDirection;
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

void UElementGameplayAbility_Meteor::SpawnMeteor(
	AActor* Caster,
	const FVector& SpawnLocation,
	const int Level
) const
{
	const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);
	if (AElementMeteor* Meteor = GetWorld()->SpawnActorDeferred<AElementMeteor>(
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
