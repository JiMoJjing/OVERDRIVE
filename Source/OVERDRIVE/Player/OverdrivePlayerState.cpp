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
}

bool AOverdrivePlayerState::IsReady() const
{
	return bIsReady;
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

void AOverdrivePlayerState::OnRep_IsReady()
{
	OnLobbyReadyChanged.Broadcast(bIsReady);
}
