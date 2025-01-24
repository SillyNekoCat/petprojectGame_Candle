// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Melee_Weapon.h"
#include "Damage_Interface.h"//
#include "Knockback_Comp.h"
#include "Sword.generated.h"


class UHeat_Component;
class UKnockback_Comp;
//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDamage_TakeW, FDamage_Inf, bool);
//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API ASword : public AMelee_Weapon, public IDamage_Interface
{
	GENERATED_BODY()

public:
	ASword();

	//validate result of trace, using him as parameter for delegate call On_SendTargets.
	virtual void Check_Hit(TArray <FHitResult> hits_results, TArray <UAbilitySystemComponent*> &ascs_apply_damage) override;

	//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
	virtual TArray <UAbilitySystemComponent*> Attack_Trace() override;

	////////////~ Begin IDamage_Interface
	//handling incoming damage specific to this class
	virtual void Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged) override;
	////////////~ End IDamage_Interface



	//FOnDamage_TakeW OnDamage_TakeWDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Trace_Radius = 36.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Trace_Complex = false;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector Shape_HalfSize {10.0, 10.0, 10.0};
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) FRotator Shape_Rotator {0.0, 0.0, 0.0};
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf <UDamageType> Impulse_DamageType;

	TArray <UAbilitySystemComponent*> ASCs_ApplyDamage;
	TArray <AActor*> Damage_Actors;
	TArray <UPrimitiveComponent*> Hit_Components;

	////////////////////COMPONENTS

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHeat_Component* Heat_Component;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
//------------------------------------------------------------------------------------------------------------
