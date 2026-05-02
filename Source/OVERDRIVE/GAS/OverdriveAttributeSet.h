// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "OverdriveAttributeSet.generated.h"

#define OVERDRIVE_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class OVERDRIVE_API UOverdriveAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UOverdriveAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "OVERDRIVE|Attributes")
	FGameplayAttributeData Health;
	OVERDRIVE_ATTRIBUTE_ACCESSORS(UOverdriveAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "OVERDRIVE|Attributes")
	FGameplayAttributeData MaxHealth;
	OVERDRIVE_ATTRIBUTE_ACCESSORS(UOverdriveAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "OVERDRIVE|Attributes")
	FGameplayAttributeData Damage;
	OVERDRIVE_ATTRIBUTE_ACCESSORS(UOverdriveAttributeSet, Damage)
};
