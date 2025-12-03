// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

private:
	float CurrentTime;
	FVector StartLocation;

public:
	AElementMeteor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
