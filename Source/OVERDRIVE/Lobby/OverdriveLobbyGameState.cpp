// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveLobbyGameState.h"

#include "Net/UnrealNetwork.h"

AOverdriveLobbyGameState::AOverdriveLobbyGameState()
{
	bReplicates = true;
}

void AOverdriveLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverdriveLobbyGameState, LobbySummary);
}

FOverdriveLobbySummary AOverdriveLobbyGameState::GetLobbySummary() const
{
	return LobbySummary;
}

int32 AOverdriveLobbyGameState::GetLobbyPlayerCount() const
{
	return LobbySummary.PlayerCount;
}

int32 AOverdriveLobbyGameState::GetReadyPlayerCount() const
{
	return LobbySummary.ReadyPlayerCount;
}

EOverdriveLobbyState AOverdriveLobbyGameState::GetLobbyState() const
{
	return LobbySummary.LobbyState;
}

void AOverdriveLobbyGameState::SetLobbySummary(int32 NewPlayerCount, int32 NewReadyPlayerCount, EOverdriveLobbyState NewLobbyState)
{
	if (!HasAuthority())
	{
		return;
	}

	if (LobbySummary.PlayerCount == NewPlayerCount && LobbySummary.ReadyPlayerCount == NewReadyPlayerCount && LobbySummary.LobbyState == NewLobbyState)
	{
		return;
	}

	LobbySummary.PlayerCount = NewPlayerCount;
	LobbySummary.ReadyPlayerCount = NewReadyPlayerCount;
	LobbySummary.LobbyState = NewLobbyState;

	BroadcastLobbySummaryChanged();
	ForceNetUpdate();
}

void AOverdriveLobbyGameState::OnRep_LobbySummary()
{
	BroadcastLobbySummaryChanged();
}

void AOverdriveLobbyGameState::BroadcastLobbySummaryChanged()
{
	OnLobbySummaryChanged.Broadcast(LobbySummary.PlayerCount, LobbySummary.ReadyPlayerCount, LobbySummary.LobbyState);
}
