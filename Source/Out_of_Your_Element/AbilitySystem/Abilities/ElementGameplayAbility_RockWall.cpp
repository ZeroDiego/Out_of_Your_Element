// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_RockWall.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"

void UElementGameplayAbility_RockWall::ActivateAbility(
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
						const FTransform MouseCursorTransform(Character->GetActorRotation() + FRotator(0, 90, 0), MouseCursorHitResult.Location);

						if (AElementWallBase* RockWall = GetWorld()->SpawnActorDeferred<AElementWallBase>(
							ElementWallBase,
							MouseCursorTransform,
							nullptr,
							nullptr,
							ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
						{
							const FGameplayEffectSpecHandle RockWallGameplayEffectSpecHandle =
								MakeOutgoingGameplayEffectSpec(
									RockWallDamageGameplayEffect,
									1);
							RockWallGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
								ElementGameplayTags::Abilities_Parameters_Damage,
								RockWallDamage);

							RockWall->InitializeZone(RockWallGameplayEffectSpecHandle, this, RockWallPopInVfx,
							                         RockWallPopOutVfx, RockWallScale,
							                         FVector(MouseCursorTransform.GetLocation().X,
							                                 MouseCursorTransform.GetLocation().Y,
							                                 MouseCursorTransform.GetLocation().Z + RockWallOffset),
							                         RockWallLifeSpan);
							UGameplayStatics::FinishSpawningActor(RockWall, MouseCursorTransform);
						}
					}
				}
			}
		}
	}

	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
