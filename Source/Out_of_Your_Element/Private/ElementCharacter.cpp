// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacter.h"
#include "ElementAbilitySystemComponent.h"

// Sets default values
AElementCharacter::AElementCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates a visible cube component in BP_ElementCharacter
	CubeRef = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cube"));

	// Attempt to find a mesh for the cube component based on the file path
    if (UStaticMesh* CubeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Cube"))))
	{
    	// If it succeeds, set the found mesh on the cube component
		CubeRef->SetStaticMesh(CubeMesh);
    	
    	// Attach the cube component to the root component
    	CubeRef->SetupAttachment(RootComponent);
	}

	// Creates a camera component in BP_ElementCharacter
	CameraRef = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	
	// Creates a spring arm component in BP_ElementCharacter
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
	FiringOffsetRef->SetupAttachment(RootComponent);
	FiringOffsetRef->SetRelativeLocation(FiringOffset);

	// Creates an ability system component in BP_ElementCharacter
	ElementAbilitySystemComponent = CreateDefaultSubobject<UElementAbilitySystemComponent>(TEXT("ElementAbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AElementCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ElementAbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& Ability : UsableAbilities)
		{
			if (Ability)
			{
				ElementAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability));
			}
		}
	}
}

// Called every frame
void AElementCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElementCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}