#include "ProjectileSpawner.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
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

    // follow the player
    if (AElementCharacter* Player = Cast<AElementCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        SetActorLocation(Player->GetActorLocation());
    }
}

void AProjectileSpawner::StartFireTimer()
{
    if (!bAutoFire) return;

    GetWorldTimerManager().SetTimer(
        FireTimerHandle,
        this,
        &AProjectileSpawner::FireAll,
        FireInterval,
        true
    );
}

void AProjectileSpawner::CreateFixedSpawnPoints()
{
    SpawnPoints.Empty();

    auto CreatePoint = [&](FString Name, FVector BaseDirection, float Offset)
    {
        USceneComponent* Comp = NewObject<USceneComponent>(this, *Name);
        Comp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Comp->RegisterComponent();

        // perpendicular offset spacing
        FVector Side(-BaseDirection.Y, BaseDirection.X, 0);
        FVector Loc = BaseDirection * SpawnDistance + Side * Offset;

        Comp->SetRelativeLocation(Loc);

        // **FLIP THE ROTATION 180 DEGREES**
        FRotator Rot = BaseDirection.Rotation();
        Rot.Yaw += 180.0f;

        Comp->SetRelativeRotation(Rot);

        SpawnPoints.Add(Comp);
    };

    const int32 CountLR = 22;
    const int32 CountUD = 18;

    auto MakeOffsets = [&](int32 Count)
    {
        TArray<float> O;
        int32 Half = Count / 2;
        float Start = (Count % 2 == 0) ? -(Half - 0.5f) : -Half;
        for (int32 i = 0; i < Count; i++)
            O.Add((Start + i) * Spacing);
        return O;
    };

    TArray<float> LR = MakeOffsets(CountLR);
    TArray<float> UD = MakeOffsets(CountUD);

    for (int32 i = 0; i < CountLR; i++)
    {
        CreatePoint("Left" + FString::FromInt(i), FVector(-1, 0, 0), LR[i]);
        CreatePoint("Right" + FString::FromInt(i), FVector(1, 0, 0), LR[i]);
    }

    for (int32 i = 0; i < CountUD; i++)
    {
        CreatePoint("Up" + FString::FromInt(i), FVector(0, 1, 0), UD[i]);
        CreatePoint("Down" + FString::FromInt(i), FVector(0, -1, 0), UD[i]);
    }
}

void AProjectileSpawner::FireAll()
{
    if (!ProjectileClass) return;

    for (USceneComponent* Point : SpawnPoints)
    {
        if (!Point) continue;

        GetWorld()->SpawnActor<AActor>(
            ProjectileClass,
            Point->GetComponentLocation(),
            Point->GetComponentRotation(),  // <-- use flipped rotation
            FActorSpawnParameters()
        );
    }
}
