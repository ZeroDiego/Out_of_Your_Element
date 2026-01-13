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

void UElementGameplayAbilityRangedSpellBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	// TODO Implement CanActivateAbility instead!
	if (ActorInfo->AvatarActor.IsValid())
	{
		const AActor* Caster = ActorInfo->AvatarActor.Get();
		if (FVector Location; GetSpellLocation(Caster, Location))
		{
			if (
				const double DistSquared = FVector::DistSquared(Caster->GetActorLocation(), Location);
				DistSquared < (MinRangedSpellPlacementRange * MinRangedSpellPlacementRange) ||
				DistSquared > (MaxRangedSpellPlacementRange * MaxRangedSpellPlacementRange)
			)
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
				return;
			}
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UElementGameplayAbilityRangedSpellBase::CastSpell(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (!ActorInfo->AvatarActor.IsValid())
	{
		return;
	}

	const AActor* Caster = ActorInfo->AvatarActor.Get();

	FVector Location;
	if (!GetSpellLocation(Caster, Location))
	{
		return;
	}

	if (
		const double DistSquared = FVector::DistSquared(Caster->GetActorLocation(), Location);
		DistSquared < (MinRangedSpellPlacementRange * MinRangedSpellPlacementRange) ||
		DistSquared > (MaxRangedSpellPlacementRange * MaxRangedSpellPlacementRange)
	)
	{
		return;
	}

	Super::CastSpell(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CastSpellAtLocation(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData,
		Location
	);
}
