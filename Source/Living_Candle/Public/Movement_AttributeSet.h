// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Movement_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//
UCLASS()
class LIVING_CANDLE_API UMovement_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move_Speed") FGameplayAttributeData Move_Speed;
	ATTRIBUTE_ACCESSORS(UMovement_AttributeSet, Move_Speed)
	///UPROPERTY(EditAnywhere, BlueprintReadWrite) bool Can_AttackAndMove = true;
};
