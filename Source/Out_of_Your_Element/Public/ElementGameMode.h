// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ElementGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AElementGameMode();
	
private:
	virtual void BeginPlay() override;
};
