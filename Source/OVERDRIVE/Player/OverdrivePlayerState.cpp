// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdrivePlayerState.h"

#include "Net/UnrealNetwork.h"

AOverdrivePlayerState::AOverdrivePlayerState()
{
	bReplicates = true;
}

void AOverdrivePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverdrivePlayerState, bIsReady);
	DOREPLIFETIME(AOverdrivePlayerState, bIsLobbyLeader);
}

bool AOverdrivePlayerState::IsReady() const
{
	return bIsReady;
}

bool AOverdrivePlayerState::IsLobbyLeader() const
{
	return bIsLobbyLeader;
}

void AOverdrivePlayerState::SetReady(bool bNewReady)
{
	if (!HasAuthority() || bIsReady == bNewReady)
	{
		return;
	}

	bIsReady = bNewReady;
	OnLobbyReadyChanged.Broadcast(bIsReady);
	ForceNetUpdate();
}

void AOverdrivePlayerState::SetLobbyLeader(bool bNewLobbyLeader)
{
	if (!HasAuthority() || bIsLobbyLeader == bNewLobbyLeader)
	{
		return;
	}

	bIsLobbyLeader = bNewLobbyLeader;
	OnLobbyLeaderChanged.Broadcast(bIsLobbyLeader);
	ForceNetUpdate();
}

void AOverdrivePlayerState::OnRep_IsReady()
{
	OnLobbyReadyChanged.Broadcast(bIsReady);
}

void AOverdrivePlayerState::OnRep_IsLobbyLeader()
{
	OnLobbyLeaderChanged.Broadcast(bIsLobbyLeader);
}
