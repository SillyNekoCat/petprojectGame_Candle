// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "WeaponAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Phys_Damage = 0.f;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Phys_Damage)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Fire_Damage = 0.f;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Fire_Damage)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Weapon_AttackSpeed_Multiplyer = 1.f;
	ATTRIBUTE_ACCESSORS(UWeaponAttributeSet, Weapon_AttackSpeed_Multiplyer)



};
