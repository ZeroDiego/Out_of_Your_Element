// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementWallBase.h"

#include "ElementProjectileBase.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void AElementWallBase::DoDamage() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AElementCharacterBase::StaticClass());

	UElementAbilitySystemComponent* CasterAsc = Caster->ElementAbilitySystemComponent;
	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor == Caster)
			continue;

		if (const AElementCharacterBase* Target = Cast<AElementCharacterBase>(OverlappingActor))
		{
			UElementAbilitySystemComponent* TargetAsc = Target->ElementAbilitySystemComponent;
			CasterAsc->BP_ApplyGameplayEffectSpecToTarget(GameplayEffectSpecHandle, TargetAsc);
		}
	}
}

void AElementWallBase::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddUniqueDynamic(this, &AElementWallBase::OnOverlap);
}

void AElementWallBase::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (const AElementProjectileBase* Projectile = Cast<AElementProjectileBase>(OtherActor))
	{
		if (const FGameplayEffectSpecHandle& GameplayEffect = Projectile->GameplayEffectSpecHandle;
			GameplayEffect.IsValid() && GameplayEffect.Data
		)
		{
			FGameplayTagContainer AssetTags;
			GameplayEffect.Data->GetAllAssetTags(AssetTags);

			if (AssetTags.HasTagExact(ElementGameplayTags::Damage_Type_Fire))
			{
				if (const float Damage = GameplayEffect.Data->GetSetByCallerMagnitude(
						ElementGameplayTags::Abilities_Parameters_Damage
					);
					Damage > 0
				)
				{
					if (FMath::IsNearlyZero(Health -= FMath::Min(Damage, Health)))
					{
						Destroy();
					}
				}
			}
		}
	}
}
