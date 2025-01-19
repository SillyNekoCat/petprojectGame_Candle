// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Melee_Weapon.h"
#include "Attack_Comp.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class LIVING_CANDLE_API UAttack_Comp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttack_Comp();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void OnMaxCombo_Reset_Index();

	UPROPERTY(BlueprintReadWrite) bool Can_Attack = true;

	UPROPERTY(BlueprintReadWrite) bool Can_AttackAndMove = false;
	
	UPROPERTY(BlueprintReadWrite) bool Is_Attacking = false;

	UPROPERTY(BlueprintReadWrite) int Attack_Index = 0;

	UPROPERTY(BlueprintReadWrite) AMelee_Weapon* Current_Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> SerialAttack_Montages;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> Attack_Montages;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;




		
};
//------------------------------------------------------------------------------------------------------------
