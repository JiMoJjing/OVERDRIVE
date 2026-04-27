// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveLobbyGameMode.h"

#include "OverdriveLobbyGameState.h"
#include "../Player/OverdrivePlayerController.h"
#include "../Player/OverdrivePlayerState.h"

AOverdriveLobbyGameMode::AOverdriveLobbyGameMode()
{
	GameStateClass = AOverdriveLobbyGameState::StaticClass();
	PlayerControllerClass = AOverdrivePlayerController::StaticClass();
	PlayerStateClass = AOverdrivePlayerState::StaticClass();
}

void AOverdriveLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	RefreshLobbySummary();
}

void AOverdriveLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	RefreshLobbySummary();
}

void AOverdriveLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	RefreshLobbySummary();
}

void AOverdriveLobbyGameMode::RefreshLobbySummary()
{
	AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();
	if (!IsValid(OverdriveLobbyGameState))
	{
		return;
	}

	int32 PlayerCount = 0;
	int32 ReadyPlayerCount = 0;

	for (APlayerState* PlayerState : OverdriveLobbyGameState->PlayerArray)
	{
		AOverdrivePlayerState* OverdrivePlayerState = Cast<AOverdrivePlayerState>(PlayerState);
		if (!IsValid(OverdrivePlayerState))
		{
			continue;
		}

		++PlayerCount;

		if (OverdrivePlayerState->IsReady())
		{
			++ReadyPlayerCount;
		}
	}

	const EOverdriveLobbyState LobbyState = PlayerCount > 0 && PlayerCount == ReadyPlayerCount ? EOverdriveLobbyState::ReadyToStart : EOverdriveLobbyState::Waiting;
	OverdriveLobbyGameState->SetLobbySummary(PlayerCount, ReadyPlayerCount, LobbyState);
}
