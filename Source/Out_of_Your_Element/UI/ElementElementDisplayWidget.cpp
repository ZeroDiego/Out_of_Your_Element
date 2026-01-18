// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementElementDisplayWidget.h"

static float GetShortestDistBetween(const float A, const float B)
{
	float Dist = A - B;
	Dist += Dist > 180 ? -360 : Dist < -180 ? 360 : 0;
	return Dist;
}

void UElementElementDisplayWidget::SetTargetRotation(const float Degrees)
{
	bIsSpinning = true;
	DegreesToSpin = GetShortestDistBetween(FRotator::NormalizeAxis(Degrees), GetCurrentRotation());
}

void UElementElementDisplayWidget::Rotate(const float Degrees)
{
	bIsSpinning = true;
	DegreesToSpin = FMath::Fmod(DegreesToSpin + Degrees, 360.0f);
}

void UElementElementDisplayWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsSpinning && !RotateTowardsTarget(RotationSpeed, InDeltaTime))
		bIsSpinning = false;
}

bool UElementElementDisplayWidget::RotateTowardsTarget(const float Speed, const float DeltaTime)
{
	const float Direction = DegreesToSpin < 0 ? -1 : 1;
	const float Spin = FMath::Abs(DegreesToSpin);
	const float RotationChange = Direction * FMath::Min(Speed * DeltaTime, Spin);
	DegreesToSpin -= RotationChange;

	SetCurrentRotation(FRotator::NormalizeAxis(GetCurrentRotation() + RotationChange));
	return !FMath::IsNearlyZero(DegreesToSpin);
}
