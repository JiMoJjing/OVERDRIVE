// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverdriveLobbyWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Player/OverdrivePlayerController.h"
#include "../Player/OverdrivePlayerState.h"

void UOverdriveLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ReadyButton))
	{
		ReadyButton->OnClicked.AddDynamic(this, &ThisClass::HandleReadyButtonClicked);
	}

	RefreshLobbyBindings();
}

void UOverdriveLobbyWidget::NativeDestruct()
{
	if (IsValid(ReadyButton))
	{
		ReadyButton->OnClicked.RemoveDynamic(this, &ThisClass::HandleReadyButtonClicked);
	}

	UnbindFromOwningPlayerState();
	UnbindFromLobbyGameState();

	Super::NativeDestruct();
}

void UOverdriveLobbyWidget::RequestSetReady(bool bNewReady)
{
	AOverdrivePlayerController* OverdrivePlayerController = GetOwningPlayer<AOverdrivePlayerController>();
	if (!IsValid(OverdrivePlayerController))
	{
		return;
	}

	OverdrivePlayerController->RequestSetReady(bNewReady);
}

void UOverdriveLobbyWidget::RefreshLobbyBindings()
{
	BindToOwningPlayerState();
	BindToLobbyGameState();

	HandleOwningPlayerReadyChanged(IsOwningPlayerReady());

	const FOverdriveLobbySummary LobbySummary = GetLobbySummary();
	HandleLobbySummaryChanged(LobbySummary.PlayerCount, LobbySummary.ReadyPlayerCount, LobbySummary.LobbyState);
}

bool UOverdriveLobbyWidget::IsOwningPlayerReady() const
{
	const AOverdrivePlayerState* OverdrivePlayerState = GetOwningPlayerState<AOverdrivePlayerState>();
	return IsValid(OverdrivePlayerState) ? OverdrivePlayerState->IsReady() : false;
}

FOverdriveLobbySummary UOverdriveLobbyWidget::GetLobbySummary() const
{
	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return FOverdriveLobbySummary();
	}

	const AOverdriveLobbyGameState* OverdriveLobbyGameState = World->GetGameState<AOverdriveLobbyGameState>();
	return IsValid(OverdriveLobbyGameState) ? OverdriveLobbyGameState->GetLobbySummary() : FOverdriveLobbySummary();
}

void UOverdriveLobbyWidget::HandleReadyButtonClicked()
{
	RequestSetReady(!IsOwningPlayerReady());
}

void UOverdriveLobbyWidget::HandleOwningPlayerReadyChanged(bool bNewReady)
{
	RefreshReadyText(bNewReady);
	OnOwningPlayerReadyChanged(bNewReady);
}

void UOverdriveLobbyWidget::HandleLobbySummaryChanged(int32 PlayerCount, int32 ReadyPlayerCount, EOverdriveLobbyState LobbyState)
{
	RefreshLobbySummaryText(PlayerCount, ReadyPlayerCount, LobbyState);
	OnLobbySummaryChanged(PlayerCount, ReadyPlayerCount, LobbyState);
}

void UOverdriveLobbyWidget::BindToOwningPlayerState()
{
	AOverdrivePlayerState* OverdrivePlayerState = GetOwningPlayerState<AOverdrivePlayerState>();
	if (BoundPlayerState == OverdrivePlayerState)
	{
		return;
	}

	UnbindFromOwningPlayerState();

	BoundPlayerState = OverdrivePlayerState;
	if (IsValid(BoundPlayerState))
	{
		BoundPlayerState->OnLobbyReadyChanged.AddDynamic(this, &ThisClass::HandleOwningPlayerReadyChanged);
	}
}

void UOverdriveLobbyWidget::BindToLobbyGameState()
{
	UWorld* World = GetWorld();
	AOverdriveLobbyGameState* OverdriveLobbyGameState = IsValid(World) ? World->GetGameState<AOverdriveLobbyGameState>() : nullptr;
	if (BoundLobbyGameState == OverdriveLobbyGameState)
	{
		return;
	}

	UnbindFromLobbyGameState();

	BoundLobbyGameState = OverdriveLobbyGameState;
	if (IsValid(BoundLobbyGameState))
	{
		BoundLobbyGameState->OnLobbySummaryChanged.AddDynamic(this, &ThisClass::HandleLobbySummaryChanged);
	}
}

void UOverdriveLobbyWidget::UnbindFromOwningPlayerState()
{
	if (IsValid(BoundPlayerState))
	{
		BoundPlayerState->OnLobbyReadyChanged.RemoveDynamic(this, &ThisClass::HandleOwningPlayerReadyChanged);
	}

	BoundPlayerState = nullptr;
}

void UOverdriveLobbyWidget::UnbindFromLobbyGameState()
{
	if (IsValid(BoundLobbyGameState))
	{
		BoundLobbyGameState->OnLobbySummaryChanged.RemoveDynamic(this, &ThisClass::HandleLobbySummaryChanged);
	}

	BoundLobbyGameState = nullptr;
}

void UOverdriveLobbyWidget::RefreshReadyText(bool bNewReady)
{
	if (IsValid(ReadyStateText))
	{
		ReadyStateText->SetText(bNewReady ? FText::FromString(TEXT("Ready: Yes")) : FText::FromString(TEXT("Ready: No")));
	}

	if (IsValid(ReadyButtonText))
	{
		ReadyButtonText->SetText(bNewReady ? FText::FromString(TEXT("Cancel Ready")) : FText::FromString(TEXT("Ready")));
	}
}

void UOverdriveLobbyWidget::RefreshLobbySummaryText(int32 PlayerCount, int32 ReadyPlayerCount, EOverdriveLobbyState LobbyState)
{
	if (IsValid(PlayerCountText))
	{
		PlayerCountText->SetText(FText::Format(FText::FromString(TEXT("Players: {0}")), FText::AsNumber(PlayerCount)));
	}

	if (IsValid(ReadyCountText))
	{
		ReadyCountText->SetText(FText::Format(FText::FromString(TEXT("Ready: {0} / {1}")), FText::AsNumber(ReadyPlayerCount), FText::AsNumber(PlayerCount)));
	}

	if (IsValid(LobbyStateText))
	{
		LobbyStateText->SetText(FText::Format(FText::FromString(TEXT("State: {0}")), GetLobbyStateText(LobbyState)));
	}
}

FText UOverdriveLobbyWidget::GetLobbyStateText(EOverdriveLobbyState LobbyState) const
{
	switch (LobbyState)
	{
	case EOverdriveLobbyState::ReadyToStart:
		return FText::FromString(TEXT("ReadyToStart"));
	case EOverdriveLobbyState::Starting:
		return FText::FromString(TEXT("Starting"));
	case EOverdriveLobbyState::Waiting:
	default:
		return FText::FromString(TEXT("Waiting"));
	}
}
