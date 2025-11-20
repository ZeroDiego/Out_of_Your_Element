// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VariableSupport/FGenericVariableStore.h"
#include "ElementGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Global variable storage. Persists while the GameInstance lives.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Variables")
	FGenericVariableStore GlobalVariables;
};
