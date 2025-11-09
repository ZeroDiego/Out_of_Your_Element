// Fill out your copyright notice in the Description page of Project Settings.


#include "DiegoCharacter.h"

// Sets default values
ADiegoCharacter::ADiegoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADiegoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADiegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADiegoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

