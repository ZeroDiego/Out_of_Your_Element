// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_FireZone.h"

#include "NiagaraFunctionLibrary.h"
#include "Engine/OverlapResult.h"
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
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const ACharacter* Character = Cast<ACharacter>(Actor))
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
						const FTransform MouseCursorTransform(FRotator(0, 0, 0), MouseCursorHitResult.Location);

						if (AElementZoneBase* FireZone = GetWorld()->SpawnActorDeferred<AElementZoneBase>(
							ElementZoneBase,
							MouseCursorTransform,
							nullptr,
							nullptr,
							ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
						{
							const FGameplayEffectSpecHandle FireZoneGameplayEffectSpecHandle =
							MakeOutgoingGameplayEffectSpec(
								FireDotDamageGameplayEffect,
								1);
							FireZoneGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
								ElementGameplayTags::Abilities_Parameters_Duration,
								FireZoneDotDamageDuration);
							FireZoneGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
								ElementGameplayTags::Abilities_Parameters_Damage,
								FireZoneDotDamage);

							FireZone->InitializeZone(FireZoneGameplayEffectSpecHandle, this, FireZoneRadius, FireZoneLifeSpan);
							UGameplayStatics::FinishSpawningActor(FireZone, MouseCursorTransform);
						}
					}
				}
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
