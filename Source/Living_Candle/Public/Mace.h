// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Melee_Weapon.h"
#include "Knockback_Comp.h"
#include "Mace.generated.h"

/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API AMace : public AMelee_Weapon
{
	GENERATED_BODY()

public:
	AMace();

	//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
	virtual void Attack_Trace() override;



	//FOnDamage_TakeW OnDamage_TakeWDelegate;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Trace_Radius = 36.0f;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector Shape_HalfSize {10.0, 10.0, 10.0};
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) FRotator Shape_Rotator {0.0, 0.0, 0.0};

	//UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf <UDamageType> Impulse_DamageType;

	////////////////////COMPONENTS


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
