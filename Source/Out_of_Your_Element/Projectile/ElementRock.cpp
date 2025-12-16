#include "ElementRock.h"

#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void MakeCasterContext(const AElementRock* Rock, FGameplayEffectContextHandle& OutContext)
{
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Rock->Caster))
	{
		if (const UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			OutContext = AbilitySystemComponent->MakeEffectContext();
		}
	}
}

void AElementRock::BeginPlay()
{
	Super::BeginPlay();

	OnProjectileHit.AddUniqueDynamic(this, &AElementRock::OnRockHit);

	if (Level >= 2)
	{
		ProjectileMovement->Bounciness = 1.0f;
		ProjectileMovement->bShouldBounce = true;
		ProjectileMovement->OnProjectileBounce.AddUniqueDynamic(this, &AElementRock::OnBounce);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst -- Used in delegate
void AElementRock::OnRockHit(const FProjectileHitEvent& Event)
{
	if (AElementCharacterBase* HitCharacter = Cast<AElementCharacterBase>(Event.HitActor))
	{
		if (!HitCharacter->ElementAbilitySystemComponent->ComponentHasTag(TEXT("State.Immune.Knockback")))
		{
			FGameplayEffectContextHandle Context;
			MakeCasterContext(this, Context);

			HitCharacter->ElementAbilitySystemComponent->BP_ApplyGameplayEffectToSelf(
				HitStunEffect,
				UGameplayEffect::INVALID_LEVEL,
				Context
			);

			HitCharacter->LaunchCharacter(
				GetActorForwardVector() * KnockbackStrength,
				true,
				true
			);
		}
	}

	if (Level >= 2)
	{
		if (Bounces < BounceCount)
		{
			UWrapperValuesLibrary::SetBoolean(Event.ShouldDestroy, false);
		}
	}
}

void AElementRock::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	++Bounces;
}
