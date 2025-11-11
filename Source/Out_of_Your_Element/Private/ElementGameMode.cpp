// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementGameMode.h"
#include "ElementCharacter.h"
#include "ElementPlayerController.h"

AElementGameMode::AElementGameMode()
{
	DefaultPawnClass = AElementCharacter::StaticClass();
	PlayerControllerClass = AElementPlayerController::StaticClass();
}

void AElementGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ADiegoGameMode::BeginPlay"));
}
