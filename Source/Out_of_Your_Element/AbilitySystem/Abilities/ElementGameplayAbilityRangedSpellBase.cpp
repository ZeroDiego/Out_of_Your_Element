// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementGameplayAbilityRangedSpellBase.h"

#include "GameFramework/Character.h"

static const FName NoPlaceTag = TEXT("Collision.Spell.BlockPlacement");

bool FTargetLocationData::HasHitResult() const
{
	return true;
}

const FHitResult* FTargetLocationData::GetHitResult() const
{
	return &HitResult;
}

UScriptStruct* FTargetLocationData::GetScriptStruct() const
{
	return StaticStruct();
}

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

bool UElementGameplayAbilityRangedSpellBase::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags
) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
		return false;

	const AActor* Caster = ActorInfo->AvatarActor.Get();
	if (FVector Location; GetSpellLocation(Caster, Location))
	{
		if (
			const double DistSquared = FVector::DistSquared(Caster->GetActorLocation(), Location);
			DistSquared < (MinRangedSpellPlacementRange * MinRangedSpellPlacementRange) ||
			DistSquared > (MaxRangedSpellPlacementRange * MaxRangedSpellPlacementRange)
		)
		{
			return false;
		}
	}

	return true;
}

void UElementGameplayAbilityRangedSpellBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	const AActor* Caster = ActorInfo->AvatarActor.Get();

	if (!TriggerEventData)
	{
		// TODO FIX THIS BAD HACK :skull:
		FGameplayEventData* GameplayEventData = new FGameplayEventData();

		FTargetLocationData* DataPtr = new FTargetLocationData();
		TraceSpell(Caster, DataPtr->HitResult);

		if (CanPlace(DataPtr->HitResult))
		{
			GameplayEventData->TargetData.Add(DataPtr);
		}
		else
		{
			delete DataPtr;
		}

		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, GameplayEventData);
		return;
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

	bool FoundLocation = false;
	FVector Location;
	if (TriggerEventData)
	{
		for (const TSharedPtr<FGameplayAbilityTargetData>& TargetData : TriggerEventData->TargetData.Data)
		{
			if (
				TargetData.IsValid() &&
				TargetData->HasHitResult() &&
				TargetData->GetScriptStruct() == FTargetLocationData::StaticStruct()
			)
			{
				Location = TargetData->GetHitResult()->ImpactPoint;
				FoundLocation = true;
				break;
			}
		}
	}

	if (FoundLocation)
	{
		delete TriggerEventData;
		TriggerEventData = nullptr;
	}

	if (!FoundLocation && !GetSpellLocation(Caster, Location))
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
