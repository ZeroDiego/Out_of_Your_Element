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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float SpawnDistance = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float Spacing = 300.f;

    // 🔥 Auto-fire timer toggle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
    bool bAutoFire = true;

    // 🔥 How often FireAll() is triggered
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing", meta = (ClampMin = "0.1"))
    float FireInterval = 3.0f;

    UFUNCTION(BlueprintCallable)
    void FireAll();

protected:
    UPROPERTY(VisibleAnywhere)
    TArray<USceneComponent*> SpawnPoints;

private:
    void CreateFixedSpawnPoints();

    FTimerHandle FireTimerHandle;
    void StartFireTimer();
};
