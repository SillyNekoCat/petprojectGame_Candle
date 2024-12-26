// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerCandle_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API UPlayerCandle_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Wick = 4.f;
	ATTRIBUTE_ACCESSORS(UPlayerCandle_AttributeSet, Wick)
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Wax_Wick_Ratio = 5.f;
	ATTRIBUTE_ACCESSORS(UPlayerCandle_AttributeSet, Wax_Wick_Ratio)
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Wick_Burn_Loss = 1.f;
	ATTRIBUTE_ACCESSORS(UPlayerCandle_AttributeSet, Wick_Burn_Loss)
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Wick_Burn_Interval = 2.f;
	ATTRIBUTE_ACCESSORS(UPlayerCandle_AttributeSet, Wick_Burn_Interval)




};
