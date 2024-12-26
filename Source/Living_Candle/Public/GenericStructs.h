// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericStructs.generated.h"
//------------------------------------------------------------------------------------------------------------
//UENUM(BlueprintType)   
//enum class Enum_Damage_Reaction : uint8
//{
//	EDefault		UMETA(DisplayName="EDefault"),	//No change reation
//	ENone			UMETA(DisplayName="ENone"),		//No reaction
//	EKnockback	UMETA(DisplayName="EKnockback"),	//Knockback reaction
//	EStun			UMETA(DisplayName="EStun"),		//Stun reaction
//};
//------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FDamage_Inf
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Pure_Damage = 0.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Fire_Damage = 0.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Phys_Damage = 0.0;
	//reaction 0 = no reaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Knockback = 0.0;
	//reaction 0 = no reaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Stun = 0.0;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere) Enum_Damage_Reaction Damage_Reaction = Enum_Damage_Reaction::ENone;//EDefault?

};

//------------------------------------------------------------------------------------------------------------
class LIVING_CANDLE_API GenericStructs
{
public:
	GenericStructs();
	~GenericStructs();
};
//------------------------------------------------------------------------------------------------------------
