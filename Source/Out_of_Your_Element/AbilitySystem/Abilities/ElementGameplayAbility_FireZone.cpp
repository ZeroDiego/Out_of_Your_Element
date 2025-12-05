// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_FireZone.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void UElementGameplayAbility_FireZone::ActivateAbility(
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
					const FTransform MouseCursorTransform(FRotator::ZeroRotator, MouseCursorHitResult.Location);

					if (AElementZoneBase* FireZone = GetWorld()->SpawnActorDeferred<AElementZoneBase>(
						FireZoneClass,
						MouseCursorTransform,
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

						FireZone->InitializeZone(
							FireZoneGameplayEffectSpecHandle,
							this,
							FireZoneVfx,
							FireZoneRadius,
							FireZoneLifeSpan
						);

						UGameplayStatics::FinishSpawningActor(FireZone, MouseCursorTransform);
					}
				}
			}
		}
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
