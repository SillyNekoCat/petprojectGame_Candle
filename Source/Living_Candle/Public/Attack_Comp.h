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

	UFUNCTION(BlueprintCallable) void Set_ActiveBlockRecoveryTimer();
	UFUNCTION(BlueprintCallable) void Clear_ActiveBlockRecoveryTimer();
	UFUNCTION() void ActiveBlockRecovery_TimerF();

	UFUNCTION(BlueprintCallable) bool ActiveBlock_ReceiveHit(AActor* causer, int break_block_value);
	UFUNCTION(BlueprintCallable) bool Is_LookAt_Block(AActor* causer, AActor* blocking_actor);


	UFUNCTION(BlueprintCallable) void OnMaxCombo_Reset_Index();
	UFUNCTION(BlueprintCallable) void Attach_Weapon(AMelee_Weapon* melee_weapon);
	UFUNCTION(BlueprintCallable) void Detach_CurrentWeapon();
	UPROPERTY(BlueprintReadWrite) bool isActive_Block = false;
	UPROPERTY(BlueprintReadWrite) bool Can_Attack = true;
	UPROPERTY(BlueprintReadWrite) bool Can_AttackAndMove = false;
	UPROPERTY(BlueprintReadWrite) bool Is_Attacking = false;

	UPROPERTY(BlueprintReadWrite) int Active_Block_Value = 1;
	UPROPERTY(BlueprintReadWrite) int MaxActive_Block_Value = 1;

	UPROPERTY(BlueprintReadWrite) int Attack_Index = 0;
	
	UPROPERTY(BlueprintReadWrite) float ActiveBlockRecovery_TimerValue = 2;
	FTimerHandle ActiveBlockRecovery_TimerHandle;
	FTimerDelegate ActiveBlockRecovery_Delegate;

	UPROPERTY(BlueprintReadWrite) AMelee_Weapon* Current_Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> SerialAttack_Montages;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> Attack_Montages;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;




		
};
//------------------------------------------------------------------------------------------------------------
