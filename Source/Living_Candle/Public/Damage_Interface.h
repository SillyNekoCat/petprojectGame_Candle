// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GenericStructs.h"
#include "Damage_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDamage_Interface : public UInterface
{
	GENERATED_BODY()
};


class LIVING_CANDLE_API IDamage_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Get_Current_Health(double &health);
	virtual void Get_Max_Health(double &max_health);
	virtual void Heal(double heal_amount, double &new_health);
	virtual void Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged);//нужно ли указывать в параметре цель или это только дл¤ блюпринтов

};
