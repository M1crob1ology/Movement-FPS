// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

ABasePlayerController::ABasePlayerController()
{
	bReplicates = true;
}

APawn* ABasePlayerController::GetPossessedPawn() const
{
	return GetPawn();
}