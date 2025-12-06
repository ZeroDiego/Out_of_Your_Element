#include "Thorn.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AThorn::AThorn()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set default offsets for Underground and AboveGround
	UndergroundOffset = FVector(0.f, 0.f, -300.f);  // Example underground offset
	AboveGroundOffset = FVector(0.f, 0.f, 0.f);    // Example above ground offset
	LerpDuration = 0.5f;  // 5 seconds for the full lerp transition
	LerpTime = 0.f;  // Initial lerp time (no movement yet)

	// Create a static mesh component for visualization
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent; // Set MeshComponent as the root of the actor
}

// Called when the game starts or when spawned
void AThorn::BeginPlay()
{
	Super::BeginPlay();

	// Store the initial position of the actor
	InitialLocation = GetActorLocation();
	
	// Set the initial position to underground (relative to the actor's position)
	SetActorLocation(InitialLocation + UndergroundOffset);
}

// Called every frame
void AThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If we are still lerping
	if (LerpTime < LerpDuration)
	{
		LerpTime += DeltaTime;  // Increase LerpTime with each frame

		// Calculate the interpolation factor
		float Alpha = FMath::Clamp(LerpTime / LerpDuration, 0.f, 1.f);

		// Linearly interpolate between the underground and above ground locations
		FVector NewLocation = FMath::Lerp(InitialLocation + UndergroundOffset, InitialLocation + AboveGroundOffset, Alpha);

		// Update the actor's location
		SetActorLocation(NewLocation);
	}
}
