// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ElementElementDisplayWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class OUT_OF_YOUR_ELEMENT_API UElementElementDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsSpinning = false;

	float DegreesToSpin;

public:
	UFUNCTION(BlueprintCallable)
	void SetTargetRotation(const float Degrees);

	UFUNCTION(BlueprintCallable)
	void Rotate(const float Degrees);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(ForceAsFunction))
	float GetCurrentRotation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentRotation(const float NewRotation);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool RotateTowardsTarget(float Speed, float DeltaTime);
};
