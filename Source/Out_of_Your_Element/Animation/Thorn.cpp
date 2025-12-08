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

	// If we're in the waiting state, count down
	if (bWaiting)
	{
		WaitTimer += DeltaTime;

		if (WaitTimer >= WaitTime)
		{
			// Stop waiting and begin downward lerp
			bWaiting = false;
			bGoingUp = false;
			LerpTime = 0.f;  
		}

		return; // Stop processing Tick until wait is done
	}


	// Handle upward or downward lerp
	if (LerpTime < LerpDuration)
	{
		LerpTime += DeltaTime;
		float Alpha = FMath::Clamp(LerpTime / LerpDuration, 0.f, 1.f);

		FVector StartLocation;
		FVector EndLocation;

		if (bGoingUp)
		{
			StartLocation = InitialLocation + UndergroundOffset;
			EndLocation = InitialLocation + AboveGroundOffset;
		}
		else
		{
			StartLocation = InitialLocation + AboveGroundOffset;
			EndLocation = InitialLocation + UndergroundOffset;
		}

		FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
		SetActorLocation(NewLocation);
	}
	else
	{
		if (bGoingUp)
		{
			// Finished going up → start waiting
			bWaiting = true;
			WaitTimer = 0.f;
		}
	
	}
}
