// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Melee_Weapon.h"
#include "Attack_Comp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActiveBlockValue_Delegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsAttacking_Change, bool, is_attack);
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

	UFUNCTION(BlueprintCallable) void Set_ActiveBlockValue(float new_value);
	UFUNCTION(BlueprintCallable) void Set_MaxActiveBlockValue(float new_value);
	UFUNCTION(BlueprintCallable) bool ActiveBlock_ReceiveHit(AActor* causer, float break_block_value);
	UFUNCTION(BlueprintCallable) bool Is_LookAt_Block(AActor* causer, AActor* blocking_actor);


	UFUNCTION(BlueprintCallable) void OnMaxLightCombo_Reset_Index();
	UFUNCTION(BlueprintCallable) void OnMaxHeavyCombo_Reset_Index();
	UFUNCTION(BlueprintCallable) void Attach_Weapon(AMelee_Weapon* melee_weapon);
	UFUNCTION(BlueprintCallable) void Detach_CurrentWeapon();
	
	UFUNCTION(BlueprintCallable) void Set_IsAttacking(bool is_attacking);
	UPROPERTY(BlueprintReadWrite) bool isActive_Block = false;
	UPROPERTY(BlueprintReadWrite) bool Can_Attack = true;
	UPROPERTY(BlueprintReadWrite) bool Can_AttackAndMove = false;
	UPROPERTY(BlueprintReadWrite) bool Is_Attacking = false;
	
	//Modify directly only as a base/start value, otherwise use a setter
	UPROPERTY(BlueprintReadOnly, EditAnywhere) float Active_Block_Value = 1;
	//Modify directly only as a base/start value, otherwise use a setter
	UPROPERTY(BlueprintReadOnly, EditAnywhere) float MaxActive_Block_Value = 1;

	UPROPERTY(BlueprintReadWrite) int Attack_Index = 0;
	
	UPROPERTY(BlueprintReadWrite) float ActiveBlockRecovery_TimerValue = 2;
	FTimerHandle ActiveBlockRecovery_TimerHandle;
	FTimerDelegate ActiveBlockRecovery_Delegate;

	UPROPERTY(BlueprintAssignable) FActiveBlockValue_Delegate On_ActiveBlockValue_Changed;
	UPROPERTY(BlueprintAssignable) FIsAttacking_Change On_IsAttacking_Change;

	UPROPERTY(BlueprintReadWrite) AMelee_Weapon* Current_Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> LightAttack_Montages;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> HeavyAttack_Montages;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)	TArray<UAnimMontage*> Attack_Montages;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
