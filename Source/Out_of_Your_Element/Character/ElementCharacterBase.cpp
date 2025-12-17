// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacterBase.h"

#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementHealthAttributeSet.h"
#include "Out_of_Your_Element/AbilitySystem/Attributes/ElementMovementAttributeSet.h"
#include "Out_of_Your_Element/AI/ElementalAIController.h"

AElementCharacterBase::AElementCharacterBase()
{
	ElementAbilitySystemComponent =
		CreateDefaultSubobject<UElementAbilitySystemComponent>(TEXT("ElementAbilitySystemComponent"));
	HealthAttributeSet = CreateDefaultSubobject<UElementHealthAttributeSet>(TEXT("Health Attribute Set"));
	MovementAttributeSet = CreateDefaultSubobject<UElementMovementAttributeSet>(TEXT("Movement Attribute Set"));
}

float AElementCharacterBase::GetHealth() const
{
	return HealthAttributeSet->GetHealth();
}

float AElementCharacterBase::IsAlive() const
{
	return GetHealth() > 0;
}

void AElementCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ElementAbilitySystemComponent->InitAbilityActorInfo(this, this);

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(
		GetAbilitySystemComponent(),
		*GetClass()->GetName(),
		1,
		true
	);

	HealthAttributeSet->InitHealth(HealthAttributeSet->GetMaxHealth());

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
		CharacterMovementComponent->MaxWalkSpeed = MovementAttributeSet->GetMovementSpeed();

	auto CheckAndApplyFreezeAndBurnImmune = [this](
		const UElementHealthAttributeSet* HealthAttributeSet,
		const FGameplayTag& DamageType,
		const FResistance& Old,
		const FResistance& New
	)
	{
		const bool bIsFire = DamageType == ElementGameplayTags::Damage_Type_Fire;
		const bool bIsFreeze = DamageType == ElementGameplayTags::Damage_Type_Water;

		if (New.DamageResistancePercent >= 1.0 && Old.DamageResistancePercent < 1.0)
		{
			UAbilitySystemComponent* Asc = HealthAttributeSet->GetOwningAbilitySystemComponent();
			const FGameplayEffectContextHandle ContextHandle = Asc->MakeEffectContext();

			if (bIsFire || bIsFreeze)
			{
				Asc->BP_ApplyGameplayEffectToSelf(
					bIsFire ? BurnImmune : FreezeImmune,
					FGameplayEffectConstants::INVALID_LEVEL,
					ContextHandle
				);
			}
		}
		else if (Old.DamageResistancePercent >= 1.0 && New.DamageResistancePercent < 1.0)
		{
			UAbilitySystemComponent* Asc = HealthAttributeSet->GetOwningAbilitySystemComponent();
			const FGameplayEffectContextHandle ContextHandle = Asc->MakeEffectContext();

			if (bIsFire || bIsFreeze)
			{
				Asc->RemoveActiveGameplayEffectBySourceEffect(bIsFire ? BurnImmune : FreezeImmune, Asc);
			}
		}
	};

	HealthAttributeSet->OnResistanceChanged.AddWeakLambda(
		this,
		CheckAndApplyFreezeAndBurnImmune
	);

	HealthAttributeSet->OnDeath.AddUniqueDynamic(this, &AElementCharacterBase::OnDeath_AddDeathEffect);
}

// ReSharper disable once CppMemberFunctionMayBeConst -- Used in delegate
void AElementCharacterBase::OnDeath_AddDeathEffect(AActor* DyingActor, const FDamageTaken& DamageTaken)
{
	UAbilitySystemComponent* Asc = HealthAttributeSet->GetOwningAbilitySystemComponent();

	Asc->BP_ApplyGameplayEffectToSelf(
		DeathEffect,
		FGameplayEffectConstants::INVALID_LEVEL,
		Asc->MakeEffectContext()
	);
}
