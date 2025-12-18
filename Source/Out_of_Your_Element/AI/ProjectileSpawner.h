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
    void RebuildSpawnPoints();

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float SpawnDistance = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    float Spacing = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    int32 LeftCount = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    bool bRightFollowLeft = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts", meta=(EditCondition="!bRightFollowLeft"))
    int32 RightCount = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    int32 DownCount = 24;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts")
    bool bUpFollowDown = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Counts", meta=(EditCondition="!bUpFollowDown"))
    int32 UpCount = 24;

    // Enable toggles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Enable")
    bool bEnableLeft = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Enable")
    bool bEnableRight = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Enable")
    bool bEnableUp = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner|Enable")
    bool bEnableDown = true;

    // Live-update system
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    bool bLiveUpdate = true;

    // firing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing", meta = (ClampMin = "0.05"))
    float FireInterval = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
    bool bAutoFire = true;

    UFUNCTION(BlueprintCallable, Category = "Firing")
    void FireAll();

    UFUNCTION(CallInEditor, Category = "Editor")
    void Editor_RebuildSpawnPoints();

protected:

    UPROPERTY()
    TArray<USceneComponent*> SpawnPoints;

private:
    void CreateSpawnPoints();
    USceneComponent* CreateSpawnComponent(const FString& Name, const FVector& RelLocation, const FRotator& RelRotation);

    // stored previous values for live change detection
    bool Prev_EnableLeft;
    bool Prev_EnableRight;
    bool Prev_EnableUp;
    bool Prev_EnableDown;
    int32 Prev_LeftCount;
    int32 Prev_RightCount;
    int32 Prev_DownCount;
    int32 Prev_UpCount;
    float Prev_SpawnDistance;
    float Prev_Spacing;

    void CacheCurrentState();
    void CheckForLiveChanges();

    FTimerHandle FireTimerHandle;
    void StartFireTimer();
};
