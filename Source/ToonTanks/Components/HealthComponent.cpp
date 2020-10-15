// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "ToonTanks/GameModes/ToonTanksGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialization outside contructor! (think componentDidMount)
	Health = DefaultHealth;
	GameModeRef = Cast<AToonTanksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// dynamic binding
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType, AController *InstigatedBy, AActor *DamageCauser)
{
	if (Damage == 0 || Health <= 0)
	{

		return;
	}

	// set boundaries so that health is not out of min/max bounds
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	if (Health <= 0)
	{
		if (GameModeRef)
		{
			GameModeRef->ActorDied(GetOwner());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Health component has no reference to the GameMode"));
		}
	}
}
