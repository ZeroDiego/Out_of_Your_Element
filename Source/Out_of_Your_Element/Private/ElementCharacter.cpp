// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacter.h"
#include "ElementAbilitySystemComponent.h"

// Sets default values
AElementCharacter::AElementCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a visible cube component in BP_DiegoCharacter
	CubeRef = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cube"));

	// Attempt to find a mesh for the cube component based on the file path
    if (UStaticMesh* CubeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Cube"))))
	{
    	// If it succeeds, set the found mesh on the cube component
		CubeRef->SetStaticMesh(CubeMesh);
    	
    	// Attach the cube component to the root component
    	CubeRef->SetupAttachment(RootComponent);
	}

	// Creates a camera component in BP_DiegoCharacter
	CameraRef = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	
	// Creates a spring arm component in BP_DiegoCharacter
	CameraBoomRef = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));

	// Attach the spring arm component to the root component
	CameraBoomRef->SetupAttachment(RootComponent);

	// Attach the camera component to the spring arm component
	CameraRef->SetupAttachment(CameraBoomRef);

	// Assign values from editable unreal properties to the spring arm component
	CameraBoomRef->TargetArmLength = TargetArmLength;
	CameraBoomRef->SocketOffset = SocketOffset;

	// Set the rotation of the camera based on editable unreal properties
	CameraRef->SetRelativeRotation(CameraRotation);

	// Creates a custom scene component called firing offset used for projectile spawn location
	FiringOffsetRef = CreateDefaultSubobject<UFiringOffset>(TEXT("FiringOffset"));
}

// Called when the game starts or when spawned
void AElementCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElementCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}