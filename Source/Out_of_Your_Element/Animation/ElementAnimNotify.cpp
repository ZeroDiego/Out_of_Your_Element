// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementAnimNotify.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"

void UElementAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (AbilityInterface->GetAbilitySystemComponent())
		{
			const FGameplayEventData EventData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), ElementGameplayTag,
			                                                         EventData);
		}
	}
}
