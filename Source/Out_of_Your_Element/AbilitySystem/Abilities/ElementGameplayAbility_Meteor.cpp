// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_Meteor.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Projectile/ElementMeteor.h"

void UElementGameplayAbility_Meteor::CastSpell(
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

						Meteor->MeteorZoneClass = MeteorZoneClass;
						Meteor->MeteorZoneVfx = MeteorZoneVfx;
						Meteor->MeteorZoneRadius = MeteorZoneRadius;
						Meteor->MeteorZoneLifeSpan = MeteorZoneLifeSpan;

						Meteor->ImpactGameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
							ImpactDamageGameplayEffect
						);

						Meteor->ImpactGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
							ElementGameplayTags::Abilities_Parameters_Damage,
							ImpactBaseDamage
						);

						Meteor->DotGameplayEffectSpecHandle =
							MakeOutgoingGameplayEffectSpec(DamageGameplayEffect);

						Meteor->DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
							ElementGameplayTags::Abilities_Parameters_Duration,
							DotDamageDuration
						);

						Meteor->DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
							ElementGameplayTags::Abilities_Parameters_Damage,
							BaseDamage
						);

						UGameplayStatics::FinishSpawningActor(Meteor, MeteorProjectileSpawnLocation);
					}
				}
			}
		}
	}
}
