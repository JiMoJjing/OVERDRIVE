// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "OverdriveHitReactInterface.h"
#include "OverdriveTargetDummy.generated.h"

class UOverdriveAttributeSet;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class OVERDRIVE_API AOverdriveTargetDummy : public AActor, public IAbilitySystemInterface, public IOverdriveHitReactInterface
{
	GENERATED_BODY()

public:
	AOverdriveTargetDummy();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void HandleWeaponHit_Implementation(AActor* InstigatorActor, const FHitResult& HitResult) override;

private:
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	void HandleDeath();

	void ApplyDeathState();

	UFUNCTION()
	void OnRep_IsDead();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|Target")
	TObjectPtr<UStaticMeshComponent> TargetMeshComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Target")
	int32 HitCount = 0;

	UPROPERTY(ReplicatedUsing = OnRep_IsDead, VisibleInstanceOnly, BlueprintReadOnly, Category = "OVERDRIVE|Target")
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OVERDRIVE|GAS")
	TObjectPtr<UOverdriveAttributeSet> AttributeSet;
};
