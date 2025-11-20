// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	FString SaveSlotName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	FString SaveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	int32 UserIndex;
	
	UMySaveGame();
};