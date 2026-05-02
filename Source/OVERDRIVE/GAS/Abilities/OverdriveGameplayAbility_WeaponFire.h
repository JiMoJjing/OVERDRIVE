// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "OverdriveGameplayAbility_WeaponFire.generated.h"

class AOverdriveCharacter;
class UGameplayEffect;

UCLASS()
class OVERDRIVE_API UOverdriveGameplayAbility_WeaponFire : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UOverdriveGameplayAbility_WeaponFire();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void StartFireRateTimer();

	void HandleFireRateElapsed();

	void SendClientTargetData(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ActivationTag);

	FGameplayAbilityTargetDataHandle MakeClientTargetData(const FGameplayAbilityActorInfo* ActorInfo) const;

	void PerformServerValidatedTrace(AOverdriveCharacter* OverdriveCharacter, const FVector& ClientAimStart, const FVector& ClientAimDirection) const;

	void ExecuteImpactGameplayCue(const FHitResult& HitResult) const;

	void ApplyDamageEffectToHitActor(AActor* HitActor) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Weapon", meta = (ClampMin = "0.0"))
	float AimTraceDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Weapon", meta = (ClampMin = "0.0"))
	float FireTraceDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Weapon", meta = (ClampMin = "0.01"))
	float FireRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "OVERDRIVE|GameplayCue")
	FGameplayTag ImpactGameplayCueTag;

	FTimerHandle FireRateTimerHandle;
};
