// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementGameplayAbilityRangedSpellBase.h"

#include "GameFramework/Character.h"

static const FName NoPlaceTag = TEXT("Collision.Spell.BlockPlacement");

FName UElementGameplayAbilityRangedSpellBase::GetNoPlaceTag()
{
	return NoPlaceTag;
}

bool UElementGameplayAbilityRangedSpellBase::TraceSpell(const AActor* Caster, FHitResult& OutHitResult)
{
	if (const ACharacter* Character = Cast<ACharacter>(Caster))
	{
		if (const AController* Controller = Character->GetController(); Controller->IsLocalPlayerController())
		{
			if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
			{
				static const TArray<TEnumAsByte<EObjectTypeQuery>> GroundTypes = {
					UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2),
				};

				return PlayerController->GetHitResultUnderCursorForObjects(
					GroundTypes,
					false,
					OutHitResult
				);
			}
		}
	}

	return false;
}

bool UElementGameplayAbilityRangedSpellBase::CanPlace(const FHitResult& HitResult)
{
	if (const AActor* Actor = HitResult.GetActor())
	{
		if (Actor->ActorHasTag(NoPlaceTag))
		{
			return false;
		}
	}

	return true;
}

bool UElementGameplayAbilityRangedSpellBase::GetSpellLocation(const AActor* Caster, FVector& OutLocation)
{
	if (FHitResult HitResult; TraceSpell(Caster, HitResult) && CanPlace(HitResult))
	{
		OutLocation = HitResult.ImpactPoint;
		return true;
	}

	return false;
}

void UElementGameplayAbilityRangedSpellBase::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::CastSpell(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo->AvatarActor.IsValid())
		return;

	FVector Location;
	if (!GetSpellLocation(ActorInfo->AvatarActor.Get(), Location))
		return;

	CastSpellAtLocation(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData,
		Location
	);
}
