// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffect.h"
#include "Animation/AnimInstance.h"
#include "ElementAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	BaseAttack = 1 UMETA(DisplayName = "BaseAttack"),
	SpecialAttack = 2 UMETA(DisplayName = "SpecialAttack"),
	HeavyAttack= 3 UMETA(DisplayName = "HeavyAttack"),
};

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EAttackType AttackType = EAttackType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> CastingGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect")
	FActiveGameplayEffectHandle ActiveCastingGameplayEffect;
};
