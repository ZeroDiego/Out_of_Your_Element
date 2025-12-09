// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementWallBase.h"

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
