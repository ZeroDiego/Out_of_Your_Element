// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementThorn.h"

#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

void AElementThorn::BeginPlay()
{
	Super::BeginPlay();

	// TODO Replace with animation trigger
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AElementThorn::DoDamage);
}

void AElementThorn::DoDamage() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AElementCharacterBase::StaticClass());

	UElementAbilitySystemComponent* CasterAsc = Caster->ElementAbilitySystemComponent;

	const FGameplayEffectSpecHandle SpecHandle = CasterAsc->MakeOutgoingSpec(
		DamageGameplayEffect,
		UGameplayEffect::INVALID_LEVEL,
		CasterAsc->MakeEffectContext()
	);

	SpecHandle.Data->SetSetByCallerMagnitude(ElementGameplayTags::Abilities_Parameters_Damage, BaseDamage);

	for (const AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor == Caster)
			continue;

		if (const AElementCharacterBase* Target = Cast<AElementCharacterBase>(OverlappingActor))
		{
			UElementAbilitySystemComponent* TargetAsc = Target->ElementAbilitySystemComponent;
			CasterAsc->BP_ApplyGameplayEffectSpecToTarget(SpecHandle, TargetAsc);
		}
	}
}
