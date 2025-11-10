// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiegoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API ADiegoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADiegoGameModeBase();
	
private:
	virtual void BeginPlay() override;
};
