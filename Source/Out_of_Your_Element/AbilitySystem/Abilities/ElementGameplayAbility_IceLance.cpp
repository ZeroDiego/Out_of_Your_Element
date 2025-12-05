// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameplayAbility_IceLance.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Out_of_Your_Element/ElementGameplayTags.h"
#include "Out_of_Your_Element/Animation/ElementAnimInstance.h"
#include "Out_of_Your_Element/Character/ElementCharacter.h"

UElementGameplayAbility_IceLance::UElementGameplayAbility_IceLance()
{
	AttackType = EAttackType::SpecialAttack;
}

void UElementGameplayAbility_IceLance::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                                       const FGameplayEventData* TriggerEventData)
{
	Super::SetAttackingTrue();

	UAbilityTask_WaitGameplayEvent* WaitCastingTimeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ElementGameplayTags::Abilities_Casting_SpecialAttack);
	WaitCastingTimeEvent->EventReceived.AddDynamic(this, &UElementGameplayAbility_IceLance::OnAnimationFinished);
	WaitCastingTimeEvent->ReadyForActivation();
}

void UElementGameplayAbility_IceLance::OnAnimationFinished(FGameplayEventData GameplayEventData)
{
	Super::OnAnimationFinished(GameplayEventData);
}

void UElementGameplayAbility_IceLance::OnAbilityFinished(FGameplayEventData GameplayEventData)
{
	Super::OnAbilityFinished(GameplayEventData);
}
