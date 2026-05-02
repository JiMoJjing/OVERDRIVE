// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdrivePlayerState.h"

#include "AbilitySystemComponent.h"
#include "../GAS/OverdriveAttributeSet.h"
#include "Net/UnrealNetwork.h"

AOverdrivePlayerState::AOverdrivePlayerState()
{
	bReplicates = true;
	SetNetUpdateFrequency(100.0f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UOverdriveAttributeSet>(TEXT("AttributeSet"));
}

void AOverdrivePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverdrivePlayerState, bIsReady);
	DOREPLIFETIME(AOverdrivePlayerState, bIsLobbyLeader);
}

UAbilitySystemComponent* AOverdrivePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UOverdriveAttributeSet* AOverdrivePlayerState::GetOverdriveAttributeSet() const
{
	return AttributeSet;
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
