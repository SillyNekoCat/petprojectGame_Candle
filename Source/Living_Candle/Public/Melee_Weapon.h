// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericStructs.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "WeaponAttributeSet.h"
#include "Attack_AttributeSet.h"
#include "Base_AttributeSet.h"
#include "Interactable_Actor.h"
#include "Components/CapsuleComponent.h"
#include "Knockback_Comp.h"
#include "Heat_Component.h"
#include "KismetTraceUtils.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Melee_Weapon.generated.h"

class UAbilitySystemComponent;
class UWeaponAttributeSet;
class UBase_AttributeSet;
class USkeletalMeshComponent;
class UInteract_SphereComponent;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API AMelee_Weapon : /*public AActor,*/ public AInteractable_Actor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	AMelee_Weapon();


	UFUNCTION() virtual void Weapon_Hit_Capsule_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);



	//UFUNCTION() void AttackTrace_Duration();

	UFUNCTION(BlueprintCallable) void Attach(USkeletalMeshComponent *arms_mesh, AActor *weapon_owner);
	UFUNCTION(BlueprintCallable) void Detach();
	UFUNCTION(BlueprintCallable) virtual void Enable_Attack_Trace();
	UFUNCTION(BlueprintCallable) virtual void Disable_Attack_Trace();

	//validate result of trace
	UFUNCTION(BlueprintCallable) virtual void Check_Hit(TArray <FHitResult> hits_results);
	//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
	UFUNCTION(BlueprintCallable) virtual void Attack_Trace(); 

	//Revert base values to Weapon_Damage_Info
	UFUNCTION(BlueprintCallable) virtual void Revert_Weapon_Damage_Info();
	//Apply Modified_ damage variables to Weapon_Damage_Info
	UFUNCTION(BlueprintCallable) virtual void Set_Weapon_BaseDamage_Info(double phys = 0.0, double fire = 0.0, double knockback = 0.0, double stun = 0.0);
	UFUNCTION(BlueprintCallable) virtual void Modify_Weapon_CurrentDamage_Info(double phys = 0.0, double fire = 0.0, double knockback = 0.0, double stun = 0.0);
	
	////////////~ Begin IAbilitySystemInterface
	// Returns our Ability System Component. 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////////~ End IAbilitySystemInterface

	
	bool Was_Weapon_Damage_Applyed;

	
	//
	FVector StartSweepLoc_Old;
	FRotator Old_ActorRotation;
	TArray <UAbilitySystemComponent*> ASCs_ApplyDamage;
	TArray <AActor*> Damage_Actors;
	TArray <UPrimitiveComponent*> Hit_Components;


	//
	UPROPERTY(BlueprintReadWrite) bool Is_SweepBegin = true;

	//Weapon debug
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug) bool Weapon_Trace_Debug = false; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Debug) float Weapon_Trace_Debug_Duration = 1.f; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Weapon_Attack = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) TSubclassOf<UGameplayEffect> GE_Damage_ToApply;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) TSubclassOf<UGameplayEffect> GE_Knockback_ToApply;
	//Need for modify
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) FDamage_Inf Weapon_CurrentDamage_Info;
	//Need for revert
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Weapon) FDamage_Inf Weapon_BaseDamage_Info;

	UPROPERTY(BlueprintReadWrite) AActor* Last_Touched_Actor = nullptr;
	UPROPERTY(BlueprintReadWrite) AActor* Owner_Of_Weapon = nullptr;

	UPROPERTY(BlueprintReadWrite) UAbilitySystemComponent* Owner_ASC = nullptr;
	
	//UPROPERTY(BlueprintReadWrite) FGameplayEffectSpec Weapon_Damage_Spec; //

	UPROPERTY(BlueprintReadOnly) TArray <AActor*> Last_Touched_Actors;
	UPROPERTY(BlueprintReadOnly) TArray <UPrimitiveComponent*> Last_Touched_Comps;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<AActor*> Weapon_Ignored_Actors;

	UPROPERTY(BlueprintReadWrite, Category = Melee_Weapon) const UAttack_AttributeSet* Owner_AttackAttributeSet;

	///////////////COMPONENTS
	UPROPERTY(BlueprintReadWrite, Category = Melee_Weapon) const UBase_AttributeSet* Base_AttributeSet;
	UPROPERTY(BlueprintReadWrite, Category = Melee_Weapon) const UWeaponAttributeSet* Weapon_AttributeSet;
	
	//UKnockback_Comp reference if it exist
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHeat_Component* Heat_Component;
	//UKnockback_Comp reference if it exist
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UKnockback_Comp* Knockback_Comp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* Weapon_Skeletal_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) UInteract_SphereComponent * Weapon_Pickup_Sphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Abilities") UAbilitySystemComponent* Weapon_AbilitySystemComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
