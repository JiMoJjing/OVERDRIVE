// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Lobby/OverdriveLobbyGameState.h"
#include "OverdriveLobbyWidget.generated.h"

UCLASS()
class OVERDRIVE_API UOverdriveLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void RequestSetReady(bool bNewReady);

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void RefreshLobbyBindings();

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	bool IsOwningPlayerReady() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	FOverdriveLobbySummary GetLobbySummary() const;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "OVERDRIVE|Lobby")
	void OnOwningPlayerReadyChanged(bool bNewReady);

	UFUNCTION(BlueprintImplementableEvent, Category = "OVERDRIVE|Lobby")
	void OnLobbySummaryChanged(int32 PlayerCount, int32 ReadyPlayerCount, EOverdriveLobbyState LobbyState);

private:
	UFUNCTION()
	void HandleReadyButtonClicked();

	UFUNCTION()
	void HandleOwningPlayerReadyChanged(bool bNewReady);

	UFUNCTION()
	void HandleLobbySummaryChanged(int32 PlayerCount, int32 ReadyPlayerCount, EOverdriveLobbyState LobbyState);

	void BindToOwningPlayerState();

	void BindToLobbyGameState();

	void UnbindFromOwningPlayerState();

	void UnbindFromLobbyGameState();

	void RefreshReadyText(bool bNewReady);

	void RefreshLobbySummaryText(int32 PlayerCount, int32 ReadyPlayerCount, EOverdriveLobbyState LobbyState);

	FText GetLobbyStateText(EOverdriveLobbyState LobbyState) const;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UButton> ReadyButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> ReadyButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> ReadyStateText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> PlayerCountText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> ReadyCountText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> LobbyStateText;

	UPROPERTY()
	TObjectPtr<class AOverdrivePlayerState> BoundPlayerState;

	UPROPERTY()
	TObjectPtr<AOverdriveLobbyGameState> BoundLobbyGameState;
};
