// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ElementThorn.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementThorn : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Thorn")
	TSubclassOf<class UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite, Category="Thorn")
	float BaseDamage;

	UPROPERTY(BlueprintReadWrite, Category="Thorn")
	const class AElementCharacterBase* Caster;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void DoDamage() const;
};
