#pragma once

#include "ElementProjectileBase.h"
#include "ElementRock.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementRock : public AElementProjectileBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rockthrow")
	TSubclassOf<UGameplayEffect> HitStunEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rockthrow")
	FVector KnockbackStrength = FVector(2000, 2000, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rockthrow Level 2")
	int BounceCount = 1;

private:
	int Bounces = 0;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRockHit(const FProjectileHitEvent& Event);

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
};
