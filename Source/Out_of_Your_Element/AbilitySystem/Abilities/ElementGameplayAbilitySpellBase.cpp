// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElementGameplayAbilitySpellBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Out_of_Your_Element/Animation/ElementAnimNotify.h"

void UElementGameplayAbilitySpellBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;

	if (!AbilityMontage)
	{
		K2_EndAbility();
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("CastSpell"),
		AbilityMontage
	);

	MontageTask->OnBlendOut.AddDynamic(this, &UElementGameplayAbilitySpellBase::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UElementGameplayAbilitySpellBase::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UElementGameplayAbilitySpellBase::K2_EndAbility);
	MontageTask->OnCompleted.AddDynamic(this, &UElementGameplayAbilitySpellBase::K2_EndAbility);

	for (const TArray<FAnimNotifyEvent>& NotifyEvents = AbilityMontage->Notifies;
	     const FAnimNotifyEvent& EventNotify : NotifyEvents
	)
	{
		if (UElementAnimNotify* const ElementAnimNotify = Cast<UElementAnimNotify>(EventNotify.Notify))
		{
			ElementAnimNotify->OnNotified.AddWeakLambda(this, [=, this](const EAnimNotifyType NotifyType)
			{
				if (NotifyType == AttackStart)
				{
					CastSpell(Handle, ActorInfo, ActivationInfo, TriggerEventData);
				}
			});
		}
	}

	MontageTask->Activate();
}
