// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Actor.h"
#include "ElementWallBase.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementWallBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite)
	const class AElementCharacterBase* Caster;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Health = 50.0f;

	UFUNCTION(BlueprintCallable)
	void DoDamage() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
