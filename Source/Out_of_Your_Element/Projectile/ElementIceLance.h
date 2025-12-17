#pragma once

#include "ElementProjectileBase.h"
#include "ElementIceLance.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementIceLance : public AElementProjectileBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Degrees"), Category="IceLance Level 2")
	float Spread = 45.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="IceLance Level 2")
	int ProjectileCount = 2;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), Category="IceLance Level 2")
	bool CanSplit = true;

protected:
	virtual void BeginPlay() override;

	void ShootIceLance(const FTransform& Transform, ACharacter* IgnoreWhenMoving) const;

private:
	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
