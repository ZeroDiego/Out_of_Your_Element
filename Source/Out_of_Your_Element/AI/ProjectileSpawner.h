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

    // Recreate spawn points (can be called from editor if needed)
    void RebuildSpawnPoints();

public:
    // Projectile to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TSubclassOf<AActor> ProjectileClass;

    // Distance from center/player where spawn points sit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float SpawnDistance = 3000.0f;

    // Spacing between spawn points along the side
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float Spacing = 300.0f;

    // Counts (Left is base for Right; Down is base for Up)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    int32 LeftCount = 6;

    // If bRightFollowLeft==true, Right points are computed as midpoints between consecutive Left points (RightCount overriden).
    // If false, RightCount is used as independent count.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    bool bRightFollowLeft = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts", meta=(EditCondition="!bRightFollowLeft"))
    int32 RightCount = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    int32 DownCount = 8;

    // If true, Up points are midpoints between consecutive Down points.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    bool bUpFollowDown = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts", meta=(EditCondition="!bUpFollowDown"))
    int32 UpCount = 8;

    // Firing control
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
    bool bAutoFire = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing", meta = (ClampMin = "0.05"))
    float FireInterval = 3.0f;

    // Manual fire
    UFUNCTION(BlueprintCallable, Category = "Firing")
    void FireAll();

    // Rebuild spawn points from editor (callable)
    UFUNCTION(CallInEditor, Category = "Editor")
    void Editor_RebuildSpawnPoints();

protected:
    // Internally stored spawn components
    UPROPERTY()
    TArray<USceneComponent*> SpawnPoints;

private:
    void CreateSpawnPoints();

    // create one spawn component, set location & rotation; returns component pointer
    USceneComponent* CreateSpawnComponent(const FString& Name, const FVector& RelLocation, const FRotator& RelRotation);

    FTimerHandle FireTimerHandle;
    void StartFireTimer();
};
