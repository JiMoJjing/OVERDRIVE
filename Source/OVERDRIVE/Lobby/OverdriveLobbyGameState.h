// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OverdriveLobbyGameState.generated.h"

UENUM(BlueprintType)
enum class EOverdriveLobbyState : uint8
{
	Waiting,
	ReadyToStart,
	Starting
};

USTRUCT(BlueprintType)
struct FOverdriveLobbySummary
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "OVERDRIVE|Lobby")
	int32 PlayerCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OVERDRIVE|Lobby")
	int32 ReadyPlayerCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "OVERDRIVE|Lobby")
	EOverdriveLobbyState LobbyState = EOverdriveLobbyState::Waiting;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLobbySummaryChanged, int32, PlayerCount, int32, ReadyPlayerCount, EOverdriveLobbyState, LobbyState);

UCLASS()
class OVERDRIVE_API AOverdriveLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AOverdriveLobbyGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	FOverdriveLobbySummary GetLobbySummary() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	int32 GetLobbyPlayerCount() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	int32 GetReadyPlayerCount() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	EOverdriveLobbyState GetLobbyState() const;

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void SetLobbySummary(int32 NewPlayerCount, int32 NewReadyPlayerCount, EOverdriveLobbyState NewLobbyState);

private:
	UFUNCTION()
	void OnRep_LobbySummary();

	void BroadcastLobbySummaryChanged();

public:
	UPROPERTY(BlueprintAssignable, Category = "OVERDRIVE|Lobby")
	FOnLobbySummaryChanged OnLobbySummaryChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_LobbySummary)
	FOverdriveLobbySummary LobbySummary;
};
