// Fill out your copyright notice in the Description page of Project Settings.


#include "DiegoCharacter.h"

// Sets default values
ADiegoCharacter::ADiegoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(FName("Cube"));
    if (UStaticMesh* CubeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Engine/BasicShapes/Cube"))))
	{
		Cube->SetStaticMesh(CubeMesh);
    	Cube->SetupAttachment(RootComponent);
	}
	
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	
	CameraBoom->SetupAttachment(RootComponent);
	Camera->SetupAttachment(CameraBoom);
	
	CameraBoom->TargetArmLength = TargetArmLength;
	CameraBoom->SocketOffset = SocketOffset;
	Camera->SetRelativeRotation(CameraRotation);
}

// Called when the game starts or when spawned
void ADiegoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADiegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}