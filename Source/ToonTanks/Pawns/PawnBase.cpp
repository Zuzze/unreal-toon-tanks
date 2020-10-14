// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APawnBase::APawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// A capsule generally used for simple collision. Bounds are rendered as lines in the editor.
	// see more at https://docs.unrealengine.com/en-US/API/Runtime/Engine/Components/UCapsuleComponent/index.html
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	// attach body of a tank to pawn's root component
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// attach turret of tank to body
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	// attach shooter into tank turret
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Update TurretMesh rotation to face target
void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// Only x and y rotation are taken into account, Z is a constant of the turret height
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);

	//Start x, y, z of turret
	FVector StartLocation = TurretMesh->GetComponentLocation();

	// change of start and target = targetV - startV
	FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();

	// set rotation to actual mesh
	TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing"))
}

void APawnBase::HandleDestruction()
{
	// play death effects particle, sound and camera shake
}
