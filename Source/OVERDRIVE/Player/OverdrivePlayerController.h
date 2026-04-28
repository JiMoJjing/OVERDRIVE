// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OverdrivePlayerController.generated.h"

UCLASS()
class OVERDRIVE_API AOverdrivePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	virtual void PostSeamlessTravel() override;

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void RequestSetReady(bool bNewReady);

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void RequestStartMission();

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void ShowLobbyWidget();

	UFUNCTION(BlueprintCallable, Category = "OVERDRIVE|Lobby")
	void HideLobbyWidget();

private:
	UFUNCTION(Server, Reliable)
	void ServerSetReady(bool bNewReady);

	UFUNCTION(Server, Reliable)
	void ServerRequestStartMission();

	UFUNCTION(Client, Reliable)
	void ClientHandlePostSeamlessTravel();

	void HandlePostSeamlessTravelOnClient();

	void RefreshLobbyWidgetBindings();

	bool ShouldShowLobbyWidget() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Lobby")
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> LobbyWidget;
};
