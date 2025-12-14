#pragma once

#include "ElementProjectileBase.h"
#include "ElementFireball.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementFireball : public AElementProjectileBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fireball")
	TSubclassOf<UGameplayEffect> DotDamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(Units="Seconds"), Category="Fireball")
	float DotDamageDuration = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fireball")
	float DotDamage = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Centimeters"), Category="Fireball Level 2")
	float SplashRadius = 250.0f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle DotGameplayEffectSpecHandle;

protected:
	virtual void BeginPlay() override;

	virtual void DoProjectileHit(AActor* HitActor) override;

private:
	UFUNCTION()
	void OnFireballHit(AElementProjectileBase* Projectile, AActor* HitActor, FMutableBool ShouldDestroy);
};
