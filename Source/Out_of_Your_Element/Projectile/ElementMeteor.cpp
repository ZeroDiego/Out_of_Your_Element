// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementMeteor.h"

AElementMeteor::AElementMeteor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElementMeteor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	SetLifeSpan(SummoningTime);
}

void AElementMeteor::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(FMath::Lerp(TargetLocation, StartLocation, GetLifeSpan() / SummoningTime));
}
