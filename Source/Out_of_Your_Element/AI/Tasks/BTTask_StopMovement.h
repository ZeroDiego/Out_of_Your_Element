// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopMovement.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UBTTask_StopMovement : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StopMovement();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};