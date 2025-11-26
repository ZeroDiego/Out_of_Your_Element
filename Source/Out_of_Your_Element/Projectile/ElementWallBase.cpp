// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementWallBase.h"

// Sets default values
AElementWallBase::AElementWallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AElementWallBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElementWallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

