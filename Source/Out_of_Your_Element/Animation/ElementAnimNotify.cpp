// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementAnimNotify.h"

void UElementAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast(NotifyType);
	Super::Notify(MeshComp, Animation, EventReference);
}
