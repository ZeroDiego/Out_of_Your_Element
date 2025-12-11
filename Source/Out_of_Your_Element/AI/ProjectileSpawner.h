#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileSpawner.generated.h"

class USceneComponent;

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AProjectileSpawner : public AActor
{
    GENERATED_BODY()

public:
    AProjectileSpawner();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TSubclassOf<AActor> ProjectileClass;

    // Distance from player/camera
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float SpawnDistance = 3000.0f;

    // Spacing for multiple spawn points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float Spacing = 300.0f;

    // Fire rate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float FireInterval = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    bool bAutoFire = true;

    UFUNCTION(BlueprintCallable)
    void FireAll();

protected:
    UPROPERTY(VisibleAnywhere)
    TArray<USceneComponent*> SpawnPoints;

private:
    void CreateFixedSpawnPoints();
    void StartFireTimer();

    FTimerHandle FireTimerHandle;
};
