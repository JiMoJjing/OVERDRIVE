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
	bUseSeamlessTravel = true;
}

void AOverdriveLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	RefreshLobbySummary();
}

void AOverdriveLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AssignLobbyLeaderIfNeeded(NewPlayer);
	RefreshLobbySummary();
}

void AOverdriveLobbyGameMode::Logout(AController* Exiting)
{
	AOverdrivePlayerState* ExitingPlayerState = IsValid(Exiting) ? Exiting->GetPlayerState<AOverdrivePlayerState>() : nullptr;
	const bool bWasLobbyLeader = IsValid(ExitingPlayerState) && ExitingPlayerState->IsLobbyLeader();

	Super::Logout(Exiting);

	if (bWasLobbyLeader)
	{
		AssignLobbyLeaderIfNeeded();
	}

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
	GetLobbyPlayerCounts(PlayerCount, ReadyPlayerCount);

	const EOverdriveLobbyState LobbyState = PlayerCount > 0 && PlayerCount == ReadyPlayerCount ? EOverdriveLobbyState::ReadyToStart : EOverdriveLobbyState::Waiting;
	OverdriveLobbyGameState->SetLobbySummary(PlayerCount, ReadyPlayerCount, LobbyState);
}

void AOverdriveLobbyGameMode::RequestStartMission(AOverdrivePlayerController* RequestingController)
{
	if (!CanStartMission(RequestingController))
	{
		return;
	}

	AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();
	if (IsValid(OverdriveLobbyGameState))
	{
		int32 PlayerCount = 0;
		int32 ReadyPlayerCount = 0;
		GetLobbyPlayerCounts(PlayerCount, ReadyPlayerCount);
		OverdriveLobbyGameState->SetLobbySummary(PlayerCount, ReadyPlayerCount, EOverdriveLobbyState::Starting);
	}

	GetWorld()->ServerTravel(MissionMapPath);
}

bool AOverdriveLobbyGameMode::CanStartMission(AOverdrivePlayerController* RequestingController) const
{
	if (!IsValid(RequestingController) || MissionMapPath.IsEmpty())
	{
		return false;
	}

	const AOverdrivePlayerState* RequestingPlayerState = RequestingController->GetPlayerState<AOverdrivePlayerState>();
	if (!IsValid(RequestingPlayerState) || !RequestingPlayerState->IsLobbyLeader())
	{
		return false;
	}

	const AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();
	if (IsValid(OverdriveLobbyGameState) && OverdriveLobbyGameState->GetLobbyState() == EOverdriveLobbyState::Starting)
	{
		return false;
	}

	int32 PlayerCount = 0;
	int32 ReadyPlayerCount = 0;
	GetLobbyPlayerCounts(PlayerCount, ReadyPlayerCount);
	return PlayerCount > 0 && PlayerCount == ReadyPlayerCount;
}

void AOverdriveLobbyGameMode::AssignLobbyLeaderIfNeeded(APlayerController* PreferredPlayer)
{
	if (IsValid(GetLobbyLeader()))
	{
		return;
	}

	AOverdrivePlayerState* NewLobbyLeader = IsValid(PreferredPlayer) ? PreferredPlayer->GetPlayerState<AOverdrivePlayerState>() : nullptr;
	AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();

	if (!IsValid(NewLobbyLeader) && IsValid(OverdriveLobbyGameState))
	{
		for (APlayerState* PlayerState : OverdriveLobbyGameState->PlayerArray)
		{
			NewLobbyLeader = Cast<AOverdrivePlayerState>(PlayerState);
			if (IsValid(NewLobbyLeader))
			{
				break;
			}
		}
	}

	if (IsValid(NewLobbyLeader))
	{
		ClearLobbyLeadersExcept(NewLobbyLeader);
		NewLobbyLeader->SetLobbyLeader(true);
	}
}

void AOverdriveLobbyGameMode::ClearLobbyLeadersExcept(AOverdrivePlayerState* LobbyLeader)
{
	AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();
	if (!IsValid(OverdriveLobbyGameState))
	{
		return;
	}

	for (APlayerState* PlayerState : OverdriveLobbyGameState->PlayerArray)
	{
		AOverdrivePlayerState* OverdrivePlayerState = Cast<AOverdrivePlayerState>(PlayerState);
		if (IsValid(OverdrivePlayerState) && OverdrivePlayerState != LobbyLeader)
		{
			OverdrivePlayerState->SetLobbyLeader(false);
		}
	}
}

void AOverdriveLobbyGameMode::GetLobbyPlayerCounts(int32& OutPlayerCount, int32& OutReadyPlayerCount) const
{
	OutPlayerCount = 0;
	OutReadyPlayerCount = 0;

	const AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();
	if (!IsValid(OverdriveLobbyGameState))
	{
		return;
	}

	for (APlayerState* PlayerState : OverdriveLobbyGameState->PlayerArray)
	{
		const AOverdrivePlayerState* OverdrivePlayerState = Cast<AOverdrivePlayerState>(PlayerState);
		if (!IsValid(OverdrivePlayerState))
		{
			continue;
		}

		++OutPlayerCount;

		if (OverdrivePlayerState->IsReady())
		{
			++OutReadyPlayerCount;
		}
	}
}

AOverdrivePlayerState* AOverdriveLobbyGameMode::GetLobbyLeader() const
{
	const AOverdriveLobbyGameState* OverdriveLobbyGameState = GetGameState<AOverdriveLobbyGameState>();
	if (!IsValid(OverdriveLobbyGameState))
	{
		return nullptr;
	}

	for (APlayerState* PlayerState : OverdriveLobbyGameState->PlayerArray)
	{
		AOverdrivePlayerState* OverdrivePlayerState = Cast<AOverdrivePlayerState>(PlayerState);
		if (IsValid(OverdrivePlayerState) && OverdrivePlayerState->IsLobbyLeader())
		{
			return OverdrivePlayerState;
		}
	}

	return nullptr;
}
