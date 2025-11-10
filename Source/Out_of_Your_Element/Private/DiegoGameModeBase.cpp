// Fill out your copyright notice in the Description page of Project Settings.


#include "DiegoGameModeBase.h"
#include "DiegoCharacter.h"
#include "DiegoPlayerController.h"

ADiegoGameModeBase::ADiegoGameModeBase()
{
	DefaultPawnClass = ADiegoCharacter::StaticClass();
	PlayerControllerClass = ADiegoPlayerController::StaticClass();
}

void ADiegoGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ADiegoGameMode::BeginPlay"));
}
