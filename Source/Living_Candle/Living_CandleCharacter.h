// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PlayerHUD.h"
#include "Attack_Comp.h"
#include "Wax_System_Comp.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Base_AttributeSet.h" 
#include "Attack_AttributeSet.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Living_CandleCharacter.generated.h"
class UBase_AttributeSet;
class UAttack_AttributeSet;
class UAbilitySystemComponent;
class UWax_System_Comp;
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AMelee_Weapon;
class APickupAble_Item;
class UWidgetComponent;
struct FInputActionValue;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
//------------------------------------------------------------------------------------------------------------
//Player Character
UCLASS(config=Game)
class ALiving_CandleCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALiving_CandleCharacter();
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	//
	UFUNCTION() void Head_Wick_Collision_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	////////////~ Begin IAbilitySystemInterface
	// Returns our Ability System Component. 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////////~ End IAbilitySystemInterface

	UPROPERTY(BlueprintReadWrite) TArray<AActor*> Interactable_Actors;

	UPROPERTY(BlueprintReadWrite) UPlayerHUD* PlayerHUD;
	
	
	//////////////////////ATTRIBUTES
	UPROPERTY(BlueprintReadOnly) const UBase_AttributeSet* Base_AttributeSet;
	UPROPERTY(BlueprintReadOnly) const UAttack_AttributeSet* Attack_AttributeSet;
	

	//////////////////////COMPONENTS

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) UAttack_Comp* Attack_Comp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Abilities") UAbilitySystemComponent* AbilitySystem_Comp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* Head_Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) UWidgetComponent* Player_Widget_HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) UWax_System_Comp* Wax_System;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true")) USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) UCameraComponent* FollowCamera;

	//////////////////////Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true")) UInputAction *Action_Use;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))UInputAction* Action_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))UInputAction* Action_AltAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))UInputAction* LookAction;


protected:
	// To add mapping context
	virtual void BeginPlay();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& value);

	/** Called for looking input */
	void Look(const FInputActionValue& value);

	void On_Action_Use(const FInputActionValue &value);

	void On_Action_Attack(const FInputActionValue &value);

	void On_Action_AltAttack(const FInputActionValue &value);

};
//------------------------------------------------------------------------------------------------------------
