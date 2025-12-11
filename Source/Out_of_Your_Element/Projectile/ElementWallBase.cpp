// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementWallBase.h"

#include "ElementProjectileBase.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"
#include "Out_of_Your_Element/Character/ElementCharacterBase.h"

AElementWallBase::AElementWallBase()
{
	ElementAbilitySystemComponent =
		CreateDefaultSubobject<UElementAbilitySystemComponent>(TEXT("ElementAbilitySystemComponent"));

	HealthAttributeSet = CreateDefaultSubobject<UElementHealthAttributeSet>(TEXT("Health Attribute Set"));
}

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

void AElementWallBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HealthAttributeSet->InitMaxHealth(DefaultHealth);
	HealthAttributeSet->InitHealth(DefaultHealth);
	HealthAttributeSet->OnHealthChanged.AddUniqueDynamic(this, &AElementWallBase::OnHealthChangeEvent);

	const FGameplayEffectContextHandle ContextHandle = ElementAbilitySystemComponent->MakeEffectContext();
	for (const auto& DefaultGameplayEffect : DefaultGameplayEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = ElementAbilitySystemComponent->MakeOutgoingSpec(
			DefaultGameplayEffect.Key,
			UGameplayEffect::INVALID_LEVEL,
			ContextHandle
		);

		SpecHandle.Data->SetByCallerTagMagnitudes = DefaultGameplayEffect.Value.Tags;
		ElementAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(SpecHandle);
	}
}

void AElementWallBase::OnHealthChangeEvent(UAttributeSet* AttributeSet, float OldValue, float NewValue)
{
	if (NewValue == 0)
	{
		Destroy();
	}
}
