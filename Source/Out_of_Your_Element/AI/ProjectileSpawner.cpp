#include "ProjectileSpawner.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Out_of_Your_Element/Character/ElementCharacter.h" // use your module/folder path

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

    // Follow player (center)
    if (AElementCharacter* Player = Cast<AElementCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        SetActorLocation(Player->GetActorLocation());
    }
}

void AProjectileSpawner::StartFireTimer()
{
    if (!bAutoFire) return;

    if (FireInterval <= 0.0f) FireInterval = 0.1f;
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AProjectileSpawner::FireAll, FireInterval, true);
    }
}

void AProjectileSpawner::Editor_RebuildSpawnPoints()
{
    // Exposed to editor to force a rebuild when tweaking values
    RebuildSpawnPoints();
}

void AProjectileSpawner::RebuildSpawnPoints()
{
    // allow caller (editor) to trigger rebuild
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
    // Clear old
    for (USceneComponent* C : SpawnPoints)
    {
        if (C) C->DestroyComponent();
    }
    SpawnPoints.Empty();

    // Helper to generate offsets array for Count using Spacing, centered
    auto GenerateOffsets = [&](int32 Count) -> TArray<float>
    {
        TArray<float> Off;
        if (Count <= 0) return Off;
        // center them: if Count even -> positions at ((-n/2 + 0.5) ... (n/2 - 0.5)) * Spacing
        int32 Half = Count / 2;
        float Start = (Count % 2 == 0) ? -(Half - 0.5f) : -Half;
        Off.Reserve(Count);
        for (int32 i = 0; i < Count; ++i)
        {
            Off.Add((Start + i) * Spacing);
        }
        return Off;
    };

    // --- HORIZONTAL: LEFT is base, RIGHT optionally midpoints ---
    // Left offsets
    TArray<float> LeftOffsets = GenerateOffsets(LeftCount);

    // create left spawn components at -X direction (Left), spaced vertically using offsets
    for (int32 i = 0; i < LeftOffsets.Num(); ++i)
    {
        FVector RelLoc = FVector(-SpawnDistance, LeftOffsets[i], 0.f);
        FRotator Rot = FVector(-1, 0, 0).Rotation();
        // flip 180 so they face inward (toward +X)
        Rot.Yaw += 180.0f;
        CreateSpawnComponent(FString::Printf(TEXT("Left_%d"), i), RelLoc, Rot);
    }

    // Right side: if follow-left, create midpoints between consecutive left offsets
    if (bRightFollowLeft)
    {
        for (int32 i = 0; i < LeftOffsets.Num() - 1; ++i)
        {
            float Mid = (LeftOffsets[i] + LeftOffsets[i + 1]) * 0.5f;
            FVector RelLoc = FVector(SpawnDistance, Mid, 0.f);
            FRotator Rot = FVector(1, 0, 0).Rotation();
            Rot.Yaw += 180.0f; // flip to face inward (toward -X)
            CreateSpawnComponent(FString::Printf(TEXT("Right_mid_%d"), i), RelLoc, Rot);
        }
    }
    else
    {
        // independent RightCount
        TArray<float> RightOffsets = GenerateOffsets(RightCount);
        for (int32 i = 0; i < RightOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(SpawnDistance, RightOffsets[i], 0.f);
            FRotator Rot = FVector(1, 0, 0).Rotation();
            Rot.Yaw += 180.0f;
            CreateSpawnComponent(FString::Printf(TEXT("Right_%d"), i), RelLoc, Rot);
        }
    }

    // --- VERTICAL: DOWN is base, UP optionally midpoints ---
    TArray<float> DownOffsets = GenerateOffsets(DownCount);

    // Down (base) placed at -Y (below center) with X offsets
    for (int32 i = 0; i < DownOffsets.Num(); ++i)
    {
        FVector RelLoc = FVector(DownOffsets[i], -SpawnDistance, 0.f);
        FRotator Rot = FVector(0, -1, 0).Rotation(); // base facing -Y
        Rot.Yaw += 180.0f; // flip to face inward (toward +Y)
        CreateSpawnComponent(FString::Printf(TEXT("Down_%d"), i), RelLoc, Rot);
    }

    if (bUpFollowDown)
    {
        for (int32 i = 0; i < DownOffsets.Num() - 1; ++i)
        {
            float Mid = (DownOffsets[i] + DownOffsets[i + 1]) * 0.5f;
            FVector RelLoc = FVector(Mid, SpawnDistance, 0.f);
            FRotator Rot = FVector(0, 1, 0).Rotation();
            Rot.Yaw += 180.0f; // flip to face inward (toward -Y)
            CreateSpawnComponent(FString::Printf(TEXT("Up_mid_%d"), i), RelLoc, Rot);
        }
    }
    else
    {
        TArray<float> UpOffsets = GenerateOffsets(UpCount);
        for (int32 i = 0; i < UpOffsets.Num(); ++i)
        {
            FVector RelLoc = FVector(UpOffsets[i], SpawnDistance, 0.f);
            FRotator Rot = FVector(0, 1, 0).Rotation();
            Rot.Yaw += 180.0f;
            CreateSpawnComponent(FString::Printf(TEXT("Up_%d"), i), RelLoc, Rot);
        }
    }
}

void AProjectileSpawner::FireAll()
{
    if (!ProjectileClass) return;
    UWorld* W = GetWorld();
    if (!W) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (USceneComponent* Comp : SpawnPoints)
    {
        if (!Comp) continue;

        const FVector SpawnLoc = Comp->GetComponentLocation();
        const FRotator SpawnRot = Comp->GetComponentRotation(); // uses component rotation (already flipped 180 when created)

        W->SpawnActor<AActor>(ProjectileClass, SpawnLoc, SpawnRot, Params);
    }
}
