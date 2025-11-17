// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "ElementAnimInstance.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttacking;
};
