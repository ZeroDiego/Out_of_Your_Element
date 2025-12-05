// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "ElementGameplayAbilityProjectileBase.h"
#include "ElementGameplayAbility_Fireball.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Fireball : public UElementGameplayAbilityProjectileBase
{
	GENERATED_BODY()

public:
	UElementGameplayAbility_Fireball();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Seconds"), Category="Fireball")
	float FireballDamageDuration;
};
