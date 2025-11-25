// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Out_of_Your_Element/System/VariableSupport/FGenericVariableStore.h"
#include "ElementSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	FString SaveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
	int32 UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Data")
    FGenericVariableStore SavedVariables;
	
	UElementSaveGame();
};