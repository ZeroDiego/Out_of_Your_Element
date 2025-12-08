// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacterBase.h"

#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}
