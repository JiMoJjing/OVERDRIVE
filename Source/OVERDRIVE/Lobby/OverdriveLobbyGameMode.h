// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OverdriveLobbyGameMode.generated.h"

class AOverdrivePlayerController;
class AOverdrivePlayerState;

UCLASS()
class OVERDRIVE_API AOverdriveLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverdriveLobbyGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void RefreshLobbySummary();

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void RequestStartMission(class AOverdrivePlayerController* RequestingController);

private:
	bool CanStartMission(AOverdrivePlayerController* RequestingController) const;

	void AssignLobbyLeaderIfNeeded(APlayerController* PreferredPlayer = nullptr);

	void ClearLobbyLeadersExcept(AOverdrivePlayerState* LobbyLeader);

	void GetLobbyPlayerCounts(int32& OutPlayerCount, int32& OutReadyPlayerCount) const;

	AOverdrivePlayerState* GetLobbyLeader() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Lobby")
	FString MissionMapPath = TEXT("/Game/Maps/MissionPrototype");
};
