// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ElementAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
	FGameplayTag ElementGameplayTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
