// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ElementAnimNotify.generated.h"



UENUM()
enum EAnimNotifyType
{
	AttackStart,
	AttackEnd
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifySignature, EAnimNotifyType)

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FOnNotifySignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EAnimNotifyType> NotifyType;
};
