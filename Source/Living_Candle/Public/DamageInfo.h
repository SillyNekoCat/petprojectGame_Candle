// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.generated.h"

USTRUCT(BlueprintType)
struct FDamage_Info
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) float Phys_Damage = 0.0;
	UPROPERTY(BlueprintReadWrite) float	Fire_Damage = 0.0;
	UPROPERTY(BlueprintReadWrite) float Pure_Damage = 0.0;
	//float Get_TotalDamage() { return Phys_Damage + Fire_Damage + Pure_Damage; };
	FDamage_Info() 
	{
		
	};
};
/**
 * 
 */
class LIVING_CANDLE_API DamageInfo
{
public:
	DamageInfo();
	~DamageInfo();
};
