// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Melee_Weapon.h"
#include "Knockback_Comp.h"
#include "Sword.generated.h"



class UKnockback_Comp;
//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDamage_TakeW, FDamage_Inf, bool);
//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API ASword : public AMelee_Weapon
{
	GENERATED_BODY()

public:
	ASword();

	//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result
	virtual void Attack_Trace() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector CollisionShape_BoxHalfExtend;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector Shape_HalfSize {10.0, 10.0, 10.0};
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) FRotator Shape_Rotator {0.0, 0.0, 0.0};
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf <UDamageType> Impulse_DamageType;

	////////////////////COMPONENTS


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
//------------------------------------------------------------------------------------------------------------
