#include "ProjectileSpawner.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Out_of_Your_Element/Character/ElementCharacter.h"

AProjectileSpawner::AProjectileSpawner()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AProjectileSpawner::BeginPlay()
{
    Super::BeginPlay();

    CreateSpawnPoints();
    CacheCurrentState();
    StartFireTimer();
}

void AProjectileSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // follow player
    if (AElementCharacter* Player = Cast<AElementCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
        SetActorLocation(Player->GetActorLocation());

    // live update
    if (bLiveUpdate)
        CheckForLiveChanges();
}

void AProjectileSpawner::CacheCurrentState()
{
    Prev_EnableLeft = bEnableLeft;
    Prev_EnableRight = bEnableRight;
    Prev_EnableUp = bEnableUp;
    Prev_EnableDown = bEnableDown;

    Prev_LeftCount = LeftCount;
    Prev_RightCount = RightCount;
    Prev_DownCount = DownCount;
    Prev_UpCount = UpCount;

    Prev_SpawnDistance = SpawnDistance;
    Prev_Spacing = Spacing;
}

void AProjectileSpawner::CheckForLiveChanges()
{
    bool bChanged =
        Prev_EnableLeft != bEnableLeft ||
        Prev_EnableRight != bEnableRight ||
        Prev_EnableUp != bEnableUp ||
        Prev_EnableDown != bEnableDown ||
        Prev_LeftCount != LeftCount ||
        Prev_RightCount != RightCount ||
        Prev_DownCount != DownCount ||
        Prev_UpCount != UpCount ||
        Prev_SpawnDistance != SpawnDistance ||
        Prev_Spacing != Spacing;

    if (bChanged)
    {
        RebuildSpawnPoints();
        CacheCurrentState();
    }
}

void AProjectileSpawner::StartFireTimer()
{
    if (!bAutoFire) return;
    if (FireInterval <= 0.0f) FireInterval = 0.1f;

    if (GetWorld())
        GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AProjectileSpawner::FireAll, FireInterval, true);
}

void AProjectileSpawner::Editor_RebuildSpawnPoints()
{
    RebuildSpawnPoints();
}

void AProjectileSpawner::RebuildSpawnPoints()
{
    CreateSpawnPoints();
}

USceneComponent* AProjectileSpawner::CreateSpawnComponent(const FString& Name, const FVector& RelLocation, const FRotator& RelRotation)
{
    USceneComponent* Comp = NewObject<USceneComponent>(this, *Name);
    Comp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Comp->RegisterComponent();
    Comp->SetRelativeLocation(RelLocation);
    Comp->SetRelativeRotation(RelRotation);
    SpawnPoints.Add(Comp);
    return Comp;
}

void AProjectileSpawner::CreateSpawnPoints()
{
    for (USceneComponent* C : SpawnPoints)
        if (C) C->DestroyComponent();
    SpawnPoints.Empty();

    auto GenerateOffsets = [&](int32 Count)
    {
        TArray<float> Off;
        if (Count <= 0) return Off;

        int32 Half = Count / 2;
        float Start = (Count % 2 == 0) ? -(Half - 0.5f) : -Half;

        for (int32 i = 0; i < Count; ++i)
            Off.Add((Start + i) * Spacing);

        return Off;
    };

    //
    // Left  = -Y
    // Right = +Y
    // Up    = -X
    // Down  = +X
    //

    // LEFT
    if (bEnableLeft)
    {
        TArray<float> LeftOffsets = GenerateOffsets(LeftCount);

        for (int32 i = 0; i < LeftOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(LeftOffsets[i], -SpawnDistance, 0.f);
            FRotator Rot = FVector(0,-1,0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Left_%d"), i), RelLoc, Rot);
        }

        if (bEnableRight)
        {
            if (bRightFollowLeft)
            {
                for (int32 i = 0; i < LeftOffsets.Num() - 1; ++i)
                {
                    float Mid = (LeftOffsets[i] + LeftOffsets[i+1]) * 0.5f;
                    FVector RelLoc = FVector(Mid, SpawnDistance, 0.f);
                    FRotator Rot = FVector(0,1,0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Right_mid_%d"), i), RelLoc, Rot);
                }
            }
            else
            {
                TArray<float> Off = GenerateOffsets(RightCount);
                for (int32 i = 0; i < Off.Num(); ++i)
                {
                    FVector RelLoc = FVector(Off[i], SpawnDistance, 0.f);
                    FRotator Rot = FVector(0,1,0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Right_%d"), i), RelLoc, Rot);
                }
            }
        }
    }
    else if (bEnableRight)
    {
        TArray<float> Off = GenerateOffsets(RightCount);
        for (int32 i = 0; i < Off.Num(); ++i)
        {
            FVector RelLoc = FVector(Off[i], SpawnDistance, 0.f);
            FRotator Rot = FVector(0,1,0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Right_%d"), i), RelLoc, Rot);
        }
    }

    // DOWN
    if (bEnableDown)
    {
        TArray<float> DownOffsets = GenerateOffsets(DownCount);

        for (int32 i = 0; i < DownOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(SpawnDistance, DownOffsets[i], 0.f);
            FRotator Rot = FVector(1,0,0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Down_%d"), i), RelLoc, Rot);
        }

        if (bEnableUp)
        {
            if (bUpFollowDown)
            {
                for (int32 i = 0; i < DownOffsets.Num() - 1; ++i)
                {
                    float Mid = (DownOffsets[i] + DownOffsets[i + 1]) * 0.5f;
                    FVector RelLoc = FVector(-SpawnDistance, Mid, 0.f);
                    FRotator Rot = FVector(-1,0,0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Up_mid_%d"), i), RelLoc, Rot);
                }
            }
            else
            {
                TArray<float> Off = GenerateOffsets(UpCount);
                for (int32 i = 0; i < Off.Num(); ++i)
                {
                    FVector RelLoc = FVector(-SpawnDistance, Off[i], 0.f);
                    FRotator Rot = FVector(-1,0,0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Up_%d"), i), RelLoc, Rot);
                }
            }
        }
    }
    else if (bEnableUp)
    {
        TArray<float> Off = GenerateOffsets(UpCount);
        for (int32 i = 0; i < Off.Num(); ++i)
        {
            FVector RelLoc = FVector(-SpawnDistance, Off[i], 0.f);
            FRotator Rot = FVector(-1,0,0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Up_%d"), i), RelLoc, Rot);
        }
    }
}

void AProjectileSpawner::FireAll()
{
    if (!ProjectileClass) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (USceneComponent* Comp : SpawnPoints)
    {
        if (!Comp) continue;
        GetWorld()->SpawnActor<AActor>(ProjectileClass,
            Comp->GetComponentLocation(),
            Comp->GetComponentRotation(),
            Params);
    }
}
