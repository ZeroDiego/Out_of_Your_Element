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
    StartFireTimer();
}

void AProjectileSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (AElementCharacter* Player = Cast<AElementCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        SetActorLocation(Player->GetActorLocation());
    }
}

void AProjectileSpawner::StartFireTimer()
{
    if (!bAutoFire) return;

    if (FireInterval <= 0.0f)
        FireInterval = 0.1f;

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

    auto GenerateOffsets = [&](int32 Count) -> TArray<float>
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
    // REAL WORLD MAPPING:
    // Left  = -Y
    // Right = +Y
    // Up    = -X
    // Down  = +X
    //

    // -------------------------- LEFT (base)  --------------------------
    if (bEnableLeft)
    {
        TArray<float> LeftOffsets = GenerateOffsets(LeftCount);

        for (int32 i = 0; i < LeftOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(LeftOffsets[i], -SpawnDistance, 0.f);
            FRotator Rot = FVector(0, -1, 0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Left_%d"), i), RelLoc, Rot);
        }

        // -------------------------- RIGHT (midpoints OR independent)  --------------------------
        if (bEnableRight)
        {
            if (bRightFollowLeft)
            {
                for (int32 i = 0; i < LeftOffsets.Num() - 1; ++i)
                {
                    float Mid = (LeftOffsets[i] + LeftOffsets[i + 1]) * 0.5f;
                    FVector RelLoc = FVector(Mid, SpawnDistance, 0.f);
                    FRotator Rot = FVector(0, 1, 0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Right_mid_%d"), i), RelLoc, Rot);
                }
            }
            else
            {
                TArray<float> RightOffsets = GenerateOffsets(RightCount);
                for (int32 i = 0; i < RightOffsets.Num(); ++i)
                {
                    FVector RelLoc = FVector(RightOffsets[i], SpawnDistance, 0.f);
                    FRotator Rot = FVector(0, 1, 0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Right_%d"), i), RelLoc, Rot);
                }
            }
        }
    }
    else if (bEnableRight) // left disabled but right enabled → independent right
    {
        TArray<float> RightOffsets = GenerateOffsets(RightCount);
        for (int32 i = 0; i < RightOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(RightOffsets[i], SpawnDistance, 0.f);
            FRotator Rot = FVector(0, 1, 0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Right_%d"), i), RelLoc, Rot);
        }
    }

    // -------------------------- DOWN (base) --------------------------
    if (bEnableDown)
    {
        TArray<float> DownOffsets = GenerateOffsets(DownCount);

        for (int32 i = 0; i < DownOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(SpawnDistance, DownOffsets[i], 0.f);
            FRotator Rot = FVector(1, 0, 0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Down_%d"), i), RelLoc, Rot);
        }

        // -------------------------- UP (midpoints OR independent) --------------------------
        if (bEnableUp)
        {
            if (bUpFollowDown)
            {
                for (int32 i = 0; i < DownOffsets.Num() - 1; ++i)
                {
                    float Mid = (DownOffsets[i] + DownOffsets[i + 1]) * 0.5f;
                    FVector RelLoc = FVector(-SpawnDistance, Mid, 0.f);
                    FRotator Rot = FVector(-1, 0, 0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Up_mid_%d"), i), RelLoc, Rot);
                }
            }
            else
            {
                TArray<float> UpOffsets = GenerateOffsets(UpCount);
                for (int32 i = 0; i < UpOffsets.Num(); ++i)
                {
                    FVector RelLoc = FVector(-SpawnDistance, UpOffsets[i], 0.f);
                    FRotator Rot = FVector(-1, 0, 0).Rotation();
                    Rot.Yaw += 180.f;
                    CreateSpawnComponent(FString::Printf(TEXT("Up_%d"), i), RelLoc, Rot);
                }
            }
        }
    }
    else if (bEnableUp) // down disabled but up enabled → independent up
    {
        TArray<float> UpOffsets = GenerateOffsets(UpCount);
        for (int32 i = 0; i < UpOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(-SpawnDistance, UpOffsets[i], 0.f);
            FRotator Rot = FVector(-1, 0, 0).Rotation();
            Rot.Yaw += 180.f;
            CreateSpawnComponent(FString::Printf(TEXT("Up_%d"), i), RelLoc, Rot);
        }
    }
}

void AProjectileSpawner::FireAll()
{
    if (!ProjectileClass) return;
    if (!GetWorld()) return;

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
