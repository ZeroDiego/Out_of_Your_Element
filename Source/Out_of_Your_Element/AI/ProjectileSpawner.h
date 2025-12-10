#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileSpawner.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AProjectileSpawner : public AActor
{
	GENERATED_BODY()

public:
	AProjectileSpawner();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// Projectile class
	UPROPERTY(EditAnywhere, Category="Spawner")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Spawner")
	float FireInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Spawner")
	float ProjectileSpeed = 1200.0f;

	// ========= Spawn point counts per side =========
	UPROPERTY(EditAnywhere, Category="Spawner Config")
	int32 LeftCount = 0;

	UPROPERTY(EditAnywhere, Category="Spawner Config")
	int32 RightCount = 0;

	UPROPERTY(EditAnywhere, Category="Spawner Config")
	int32 TopCount = 0;

	UPROPERTY(EditAnywhere, Category="Spawner Config")
	int32 BottomCount = 0;

private:
	FTimerHandle FireTimer;

	// All dynamically created spawn components
	UPROPERTY()
	TArray<USceneComponent*> SpawnPoints;

	void GenerateSpawnPoints();
	void FireAllPoints();
	void FireFrom(USceneComponent* SpawnPoint);
};