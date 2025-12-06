#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thorn.generated.h"

UCLASS()
class OUT_OF_YOUR_ELEMENT_API AThorn : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AThorn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// The offsets for the underground and above ground locations relative to the actor's initial position
	UPROPERTY(EditAnywhere)
	FVector UndergroundOffset;

	UPROPERTY(EditAnywhere)
	FVector AboveGroundOffset;

	// The duration to move from underground to above ground
	UPROPERTY(EditAnywhere)
	float LerpDuration;

	// Internal timer to keep track of progress
	float LerpTime;

	// The actor's mesh (or root) that will be moved
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	// The initial position of the actor
	FVector InitialLocation;
};
