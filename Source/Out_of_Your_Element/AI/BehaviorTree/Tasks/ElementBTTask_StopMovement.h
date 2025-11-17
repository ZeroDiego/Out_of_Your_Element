// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ElementBTTask_StopMovement.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API UElementBTTask_StopMovement : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UElementBTTask_StopMovement();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
