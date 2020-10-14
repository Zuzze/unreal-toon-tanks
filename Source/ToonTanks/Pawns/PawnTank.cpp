// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// constructor
APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction();
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Rotate();
    Move();

    if (PlayerControllerRef)
    {
        FHitResult TraceHitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Input mapping names from project settings
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);

    // PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APawnTank::BeginCrouch);
    // PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APawnTank::LeaveCrouch);
}

/* Return */
void APawnTank::CalculateMoveInput(float Value)
{
    // move on x axis
    // velocity = s = d / t = distance / time
    // => d = distance = velocity * time = s * t
    MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value)
{
    float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
    // rotation on yaw (z-axis) is same as rotation on the ground
    FRotator Rotation = FRotator(0, RotateAmount, 0);
    RotationDirection = FQuat(Rotation);
}

void APawnTank::Move()
{
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
    AddActorLocalRotation(RotationDirection, true);
}
