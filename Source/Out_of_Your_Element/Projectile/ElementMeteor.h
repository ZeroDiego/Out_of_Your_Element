// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "ElementMeteor.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementMeteor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	float SummoningTime;

	UPROPERTY(BlueprintReadOnly)
	FVector TargetLocation;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class AElementZoneBase> MeteorZoneClass;

	UPROPERTY()
	class UNiagaraSystem* MeteorZoneVfx;

	UPROPERTY()
	float MeteorZoneRadius = 250;

	UPROPERTY()
	float MeteorZoneLifeSpan = 10;

	UPROPERTY()
	FGameplayEffectSpecHandle DotGameplayEffectSpecHandle;

	UPROPERTY()
	FGameplayEffectSpecHandle ImpactGameplayEffectSpecHandle;

private:
	float CurrentTime;
	FVector StartLocation;

public:
	AElementMeteor();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void LifeSpanExpired() override;
};
