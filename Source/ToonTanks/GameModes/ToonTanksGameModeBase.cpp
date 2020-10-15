// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksGameModeBase.h"

void AToonTanksGameModeBase::BeginPlay()
{
    // Get references
}

void AToonTanksGameModeBase::ActorDied(AActor *DeadActor)
{
    UE_LOG(LogTemp, Warning, TEXT("A Pawn died"));
}

void AToonTanksGameModeBase::HandleGameStart()
{
}

void AToonTanksGameModeBase::HandleGameOver(bool PlayerWon)
{
}
