// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Attack_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API UAttack_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAttack_AttributeSet();


	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Melee_Damage = 1.f;
	ATTRIBUTE_ACCESSORS(UAttack_AttributeSet, Melee_Damage)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Range_Damage = 1.f;
	ATTRIBUTE_ACCESSORS(UAttack_AttributeSet, Range_Damage)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Added_Physical_Damage = 0.f;
	ATTRIBUTE_ACCESSORS(UAttack_AttributeSet, Added_Physical_Damage)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Physical_Damage_Multiplier = 1.f;
	ATTRIBUTE_ACCESSORS(UAttack_AttributeSet, Physical_Damage_Multiplier)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Knockback = 0.f;
	ATTRIBUTE_ACCESSORS(UAttack_AttributeSet, Knockback)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Stun = 0.f;
	ATTRIBUTE_ACCESSORS(UAttack_AttributeSet, Stun)

};
