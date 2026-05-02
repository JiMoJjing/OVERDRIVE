// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "OverdrivePlayerState.generated.h"

class UAbilitySystemComponent;
class UOverdriveAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyReadyChanged, bool, bNewReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyLeaderChanged, bool, bNewLobbyLeader);

UCLASS()
class OVERDRIVE_API AOverdrivePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AOverdrivePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|GAS")
	UOverdriveAttributeSet* GetOverdriveAttributeSet() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	bool IsReady() const;

	UFUNCTION(BlueprintPure, Category = "OVERDRIVE|Lobby")
	bool IsLobbyLeader() const;

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void SetReady(bool bNewReady);

	UFUNCTION(BlueprintAuthorityOnly, Category = "OVERDRIVE|Lobby")
	void SetLobbyLeader(bool bNewLobbyLeader);

private:
	UFUNCTION()
	void OnRep_IsReady();

	UFUNCTION()
	void OnRep_IsLobbyLeader();

public:
	UPROPERTY(BlueprintAssignable, Category = "OVERDRIVE|Lobby")
	FOnLobbyReadyChanged OnLobbyReadyChanged;

	UPROPERTY(BlueprintAssignable, Category = "OVERDRIVE|Lobby")
	FOnLobbyLeaderChanged OnLobbyLeaderChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsLobbyLeader)
	bool bIsLobbyLeader = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverdriveAttributeSet> AttributeSet;
};
