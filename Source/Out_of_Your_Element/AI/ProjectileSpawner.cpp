#include "ProjectileSpawner.h"
#include "Components/SceneComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

AProjectileSpawner::AProjectileSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

void AProjectileSpawner::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    GenerateSpawnPoints();
}

void AProjectileSpawner::GenerateSpawnPoints()
{
    // Remove old components
    for (USceneComponent* Comp : SpawnPoints)
    {
        if (Comp)
        {
            Comp->DestroyComponent();
        }
    }
    SpawnPoints.Empty();

    auto CreatePoints = [&](int Count, const FString& Prefix)
    {
        for (int i = 0; i < Count; i++)
        {
            FString Name = Prefix + "_" + FString::FromInt(i + 1);

            USceneComponent* Point = NewObject<USceneComponent>(this, *Name);
            Point->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

            Point->RegisterComponent();

            // Default positions can be whatever you want
            // e.g. left and right positioned automatically
            if (Prefix == "Left")    Point->SetRelativeLocation(FVector(0, -500, 0));
            if (Prefix == "Right")   Point->SetRelativeLocation(FVector(0, 500, 0));
            if (Prefix == "Top")     Point->SetRelativeLocation(FVector(500, 0, 0));
            if (Prefix == "Bottom")  Point->SetRelativeLocation(FVector(-500, 0, 0));

            SpawnPoints.Add(Point);
        }
    };

    CreatePoints(LeftCount,   "Left");
    CreatePoints(RightCount,  "Right");
    CreatePoints(TopCount,    "Top");
    CreatePoints(BottomCount, "Bottom");
}

void AProjectileSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (ProjectileClass)
    {
        GetWorld()->GetTimerManager().SetTimer(
            FireTimer,
            this,
            &AProjectileSpawner::FireAllPoints,
            FireInterval,
            true
        );
    }
}

void AProjectileSpawner::FireAllPoints()
{
    for (USceneComponent* Point : SpawnPoints)
    {
        FireFrom(Point);
    }
}

void AProjectileSpawner::FireFrom(USceneComponent* SpawnPoint)
{
    if (!SpawnPoint || !ProjectileClass) return;

    const FVector Loc = SpawnPoint->GetComponentLocation();
    const FRotator Rot = SpawnPoint->GetComponentRotation();

    AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, Loc, Rot);

    if (UProjectileMovementComponent* Move = Projectile->FindComponentByClass<UProjectileMovementComponent>())
    {
        Move->Velocity = SpawnPoint->GetForwardVector() * ProjectileSpeed;
    }
}
