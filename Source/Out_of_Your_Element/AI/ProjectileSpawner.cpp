#include "ProjectileSpawner.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Out_of_Your_Element/Character/ElementCharacter.h"

AProjectileSpawner::AProjectileSpawner()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AProjectileSpawner::BeginPlay()
{
    Super::BeginPlay();

    CreateFixedSpawnPoints();

    StartFireTimer();
}

void AProjectileSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Follow the player
    if (AElementCharacter* Player = Cast<AElementCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        SetActorLocation(Player->GetActorLocation());
    }
}

void AProjectileSpawner::StartFireTimer()
{
    if (!bAutoFire)
        return;

    if (FireInterval > 0.f)
    {
        GetWorldTimerManager().SetTimer(
            FireTimerHandle,
            this,
            &AProjectileSpawner::FireAll,
            FireInterval,
            true
        );
    }
}

void AProjectileSpawner::CreateFixedSpawnPoints()
{
    SpawnPoints.Empty();

    auto CreatePoint = [&](FString Name, FVector Direction, float Offset)
    {
        USceneComponent* Comp = NewObject<USceneComponent>(this, *Name);
        Comp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Comp->RegisterComponent();

        FVector Perpendicular(-Direction.Y, Direction.X, 0);
        FVector Location = Direction * SpawnDistance + Perpendicular * Offset;

        Comp->SetRelativeLocation(Location);
        Comp->SetRelativeRotation(Direction.Rotation());

        SpawnPoints.Add(Comp);
    };

    TArray<float> Offsets = { -1.5f * Spacing, -0.5f * Spacing, 0.5f * Spacing, 1.5f * Spacing };

    for (int i = 0; i < 4; i++)
    {
        CreatePoint("Left_" + FString::FromInt(i), FVector(-1, 0, 0), Offsets[i]);
        CreatePoint("Right_" + FString::FromInt(i), FVector(1, 0, 0), Offsets[i]);
        CreatePoint("Up_" + FString::FromInt(i), FVector(0, 1, 0), Offsets[i]);
        CreatePoint("Down_" + FString::FromInt(i), FVector(0, -1, 0), Offsets[i]);
    }
}

void AProjectileSpawner::FireAll()
{
    if (!ProjectileClass)
        return;

    for (USceneComponent* Point : SpawnPoints)
    {
        if (!Point)
            continue;

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(
            ProjectileClass,
            Point->GetComponentLocation(),
            Point->GetComponentRotation(),
            Params
        );

        // Debug visualization (optional)
        //DrawDebugSphere(GetWorld(), Point->GetComponentLocation(), 32, 12, FColor::Red, false, 1.0f);
    }
}
