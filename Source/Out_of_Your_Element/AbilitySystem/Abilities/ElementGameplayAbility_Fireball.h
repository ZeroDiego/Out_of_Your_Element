// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "ElementGameplayAbilityProjectileSpellBase.h"
#include "ElementGameplayAbility_Fireball.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameplayAbility_Fireball : public UElementGameplayAbilityProjectileSpellBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Seconds"), Category="Fireball")
	float FireballDamageDuration;
};
