// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericStructs.h"
//#include "Damage_Interface.h"
#include "Interactable_Actor.h"
#include "Components/CapsuleComponent.h"
#include "Knockback_Comp.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Melee_Weapon.generated.h"

class USkeletalMeshComponent;
class UInteract_SphereComponent;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API AMelee_Weapon : /*public AActor,*/ public AInteractable_Actor
{
	GENERATED_BODY()

public:	
	AMelee_Weapon();


	UFUNCTION() virtual void Weapon_Hit_Capsule_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//Looping function using for trace, need to extend or override in child classes
	UFUNCTION(BlueprintCallable) virtual void Attack_Trace(); 

	//UFUNCTION() void AttackTrace_Duration();

	UFUNCTION(BlueprintCallable) void Attach(USkeletalMeshComponent *arms_mesh, AActor *weapon_owner);
	UFUNCTION(BlueprintCallable) void Detach();
	UFUNCTION(BlueprintCallable) virtual void Enable_Attack_Trace();
	UFUNCTION(BlueprintCallable) virtual void Disable_Attack_Trace();

	//function using for calculate melee attack
	UFUNCTION(BlueprintCallable) virtual void Check_Hit(TArray <FHitResult> hits_results);
	//Revert base values to Weapon_Damage_Info
	UFUNCTION(BlueprintCallable) virtual void Revert_Weapon_Damage_Info();
	//Apply Modified_ damage variables to Weapon_Damage_Info
	UFUNCTION(BlueprintCallable) virtual void Set_Weapon_BaseDamage_Info(double phys = 0.0, double fire = 0.0, double knockback = 0.0, double stun = 0.0);
	UFUNCTION(BlueprintCallable) virtual void Modify_Weapon_CurrentDamage_Info(double phys = 0.0, double fire = 0.0, double knockback = 0.0, double stun = 0.0);
	
	bool Was_Weapon_Damage_Applyed;


	//FTimerHandle Trace_TimerHandle; LOOPING TIMER TRACE METHOD
	//FTimerDelegate Trace_Delegate; LOOPING TIMER TRACE METHOD

	//FTimerHandle AttackDuration_TimerHandle;
	//FTimerDelegate AttackDuration_Delegate;

	//Weapon debug
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug) bool Weapon_Trace_Debug = false; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug) float Weapon_Trace_Debug_Duration = 1.f; 

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) float Attack_Trace_Interval = 0.01f; LOOPING TIMER TRACE METHOD

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug) bool Is_Weapon_Attack = false;
	//Need for modify
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) FDamage_Inf Weapon_CurrentDamage_Info;
	//Need for revert
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) FDamage_Inf Weapon_BaseDamage_Info;

	UPROPERTY(BlueprintReadWrite) AActor* Last_Touched_Actor = nullptr;
	UPROPERTY(BlueprintReadWrite) AActor* Owner_Of_Weapon = nullptr;

	UPROPERTY(BlueprintReadOnly) TArray <AActor*> Last_Touched_Actors;
	UPROPERTY(BlueprintReadOnly) TArray <UPrimitiveComponent*> Last_Touched_Comps;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<AActor*> Weapon_Ignored_Actors;
	
	//UKnockback_Comp reference if it exist
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UKnockback_Comp* Knockback_Comp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* Weapon_Skeletal_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) UInteract_SphereComponent * Weapon_Pickup_Sphere;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
