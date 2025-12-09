// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_RockWall.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void UElementGameplayAbility_RockWall::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (AActor* Actor = GetAvatarActorFromActorInfo())
	{
		if (const AElementCharacterBase* Caster = Cast<AElementCharacterBase>(Actor))
		{
			if (const APlayerController* PlayerController = Cast<APlayerController>(Caster->GetController()))
			{
				if (PlayerController->IsLocalPlayerController())
				{
					static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
						UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2),
					};

					if (FHitResult MouseCursorHitResult; PlayerController->GetHitResultUnderCursorForObjects(
						GroundTypes, false, MouseCursorHitResult))
					{
						const FTransform MouseCursorTransform(
							Caster->GetActorRotation(),
							MouseCursorHitResult.Location
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
			}
		}
	}
}
