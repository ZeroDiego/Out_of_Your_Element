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

void AElementFireball::DoProjectileHit(AActor* HitActor)
{
	Super::DoProjectileHit(HitActor);

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
			{HitActor, Caster},
			HitActors
		);

		for (AActor* const& NearbyActor : HitActors)
			Super::DoProjectileHit(NearbyActor);
	}
}

void AElementFireball::OnFireballHit(
	AElementProjectileBase* Projectile,
	AActor* HitActor,
	FMutableBool ShouldDestroy
)
{
	if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(HitActor))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(DotGameplayEffectSpecHandle);
		}
	}
}
