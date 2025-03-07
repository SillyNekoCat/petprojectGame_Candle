// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Base_AttributeSet.h" 
#include "Interact_CapsuleComponent.h"
#include "Interact_BoxComponent.h"
#include "Interact_SphereComponent.h"
//#include "Components/ShapeComponent.h"
#include "Heat_Actor.generated.h"

//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDamage_Take, FDamage_Inf, bool);//TEMP old
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamage_Take, FDamage_Inf, damage_info, bool, was_damage_taken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheck_Heat, double, heat_status);
class UBase_AttributeSet;
class UAbilitySystemComponent;
class UHeat_Component;
//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API AHeat_Actor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	AHeat_Actor(); 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void Collision_ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable) void Set_CheckContactDamage_Timer();
	// timer function
	UFUNCTION(BlueprintCallable) void HeatContact_Damage(); 
	UFUNCTION(BlueprintCallable) void Clear_CheckContactDamage_Timer();
	

	//UFUNCTION(BlueprintCallable) void Apply_Damage(AActor* target, FDamage_Inf damage_info, bool& was_damage_applyed); 



	
	////////////~ Begin IAbilitySystemInterface
	// Returns our Ability System Component. 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////////~ End IAbilitySystemInterface

	UFUNCTION(BlueprintCallable) void Death();
	UFUNCTION(BlueprintCallable) void Calculate_Melting_Scale(FVector old_scale, FRotator actor_rotation, FVector& result_melting_scale);
	UFUNCTION(BlueprintCallable) void Check_HeatMelting();
	
	FVector Get_Shape_Value(UBoxComponent* collision_comp);
	void Correct_Collision(UBoxComponent* collision_comp, FVector scale);//Корректирует компонент коллизии после скейла в зависимости от его типа. 

	//using for debug
	bool do_once = true;
	////DEBUG
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Debug = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Debug_Screen = false;

	
	FVector Start_BoxExtent;

	FVector Start_ActorScale;


	FTimerHandle CheckContactDamage_TimerHandle;
	FTimerDelegate CheckContactDamage_Delegate;

	UBoxComponent* Box_Comp; //a component found in the BP hierarchy, need for Correct_Collision

	//UPROPERTY(BlueprintAssignable) FOnDamage_Take OnDamage_TakeDelegate;//Need for Heat_Component or level scripting
	// 
	//Need for level scripting or...
	UPROPERTY(BlueprintAssignable) FOnCheck_Heat OnCheckHeat_Delegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf<UGameplayEffect> GE_HeatContactDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Can_Melting = false; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Melting = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Death_OnMaxHeat = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Required_HeatStatus_ForMelting = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Melting_InterpSpeed = 2.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float ContactCheck_Interval = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Melting_Scale_Divider = 2.0;

	UPROPERTY(BlueprintReadWrite) UPrimitiveComponent* Last_OverlapComp;

	//////////////Components

	UPROPERTY(BlueprintReadOnly) const UBase_AttributeSet* Base_AttributeSet;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Abilities") UAbilitySystemComponent* AbilitySystem_Comp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UHeat_Component* Heat_Component;


protected:
	virtual void BeginPlay() override;// Called when the game starts or when spawned



};
//------------------------------------------------------------------------------------------------------------
