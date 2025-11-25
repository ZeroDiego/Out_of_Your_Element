// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementCharacterBase.h"

#include "NiagaraFunctionLibrary.h"

// Sets default values
AElementCharacterBase::AElementCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireDotNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraComponent"));
	FireDotNiagaraComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AElementCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	FireDotNiagaraComponent->SetVisibility(false);
	FireDotNiagaraComponent->SetAsset(FireDotVfx);
}

void AElementCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AElementCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (FGameplayTag Tag : GetAbilitySystemComponent()->GetOwnedGameplayTags())
	{
		if (Tag.IsValid())
		{
			if (Tag.GetTagName() == TEXT("Abilities.Fire"))
			{
				FireDotNiagaraComponent->SetVisibility(true);
				return;
			}
		}
	}

	FireDotNiagaraComponent->SetVisibility(false);
}

// Called to bind functionality to input
void AElementCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
