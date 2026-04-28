// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveMissionGameMode.h"

#include "OverdriveMissionGameState.h"
#include "../Player/OverdrivePlayerController.h"
#include "../Player/OverdrivePlayerState.h"

AOverdriveMissionGameMode::AOverdriveMissionGameMode()
{
	GameStateClass = AOverdriveMissionGameState::StaticClass();
	PlayerControllerClass = AOverdrivePlayerController::StaticClass();
	PlayerStateClass = AOverdrivePlayerState::StaticClass();
	bUseSeamlessTravel = true;
}
