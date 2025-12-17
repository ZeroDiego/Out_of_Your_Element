#include "ElementFireball.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AI/ElementAICharacterBase.h"

void AElementFireball::BeginPlay()
{
	Super::BeginPlay();

	if (!DotGameplayEffectSpecHandle.IsValid())
	{
		if (const UGameplayEffect* DotDamageEffectCDO = DotDamageGameplayEffect.GetDefaultObject())
		{
			FGameplayEffectContext* EffectContext = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
			EffectContext->AddInstigator(Caster, this);
			DotGameplayEffectSpecHandle = FGameplayEffectSpecHandle(
				new FGameplayEffectSpec(DotDamageEffectCDO, FGameplayEffectContextHandle(EffectContext))
			);

			DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Duration,
				DotDamageDuration
			);

			DotGameplayEffectSpecHandle.Data->SetSetByCallerMagnitude(
				ElementGameplayTags::Abilities_Parameters_Damage,
				DotDamage
			);
		}
	}

	OnProjectileHit.AddUniqueDynamic(this, &AElementFireball::OnFireballHit);
}

void AElementFireball::DoProjectileHit(const FProjectileHitEvent& PreEvent)
{
	Super::DoProjectileHit(PreEvent);

	if (Level >= 2)
	{
		static const TArray<TEnumAsByte<EObjectTypeQuery>> SplashTypes = {
			UEngineTypes::ConvertToObjectType(ECC_Pawn)
		};

		TArray<AActor*> HitActors;
		UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			GetActorLocation(),
			SplashRadius,
			SplashTypes,
			AElementAICharacterBase::StaticClass(),
			{PreEvent.HitActor, Caster},
			HitActors
		);

		for (AActor* const& NearbyActor : HitActors)
			Super::DoProjectileHit(PreEvent);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst -- Used in delegate
void AElementFireball::OnFireballHit(
	const FProjectileHitEvent& Event
)
{
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Event.HitActor))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(DotGameplayEffectSpecHandle);
		}
	}
}
