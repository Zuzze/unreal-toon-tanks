// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void AToonTanksGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void AToonTanksGameModeBase::ActorDied(AActor *DeadActor)
{
    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        if (PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret *DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();

        if (--TargetTurrets == 0)
        {
            HandleGameOver(true);
        }
    }
}

void AToonTanksGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();
    // enable player controls
    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);
        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, &APlayerControllerBase::SetPlayerEnabledState, true);
        // Set timeout so that player cannot move unti timer countdown finished
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

void AToonTanksGameModeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}

int32 AToonTanksGameModeBase::GetTargetTurretCount()
{
    // get number of turrets on the level (current world)
    TArray<AActor *> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    return TurretActors.Num();
}
