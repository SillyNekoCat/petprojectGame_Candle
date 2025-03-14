// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/CapsuleComponent.h"
#include "Fire_Stream.generated.h"

class UCapsuleComponent;
class UAbilitySystemComponent;
class USceneComponent;

UCLASS()
class LIVING_CANDLE_API AFire_Stream : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AFire_Stream();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void Capsule_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable) void Check_EnableOrDisable();
	UFUNCTION(BlueprintCallable) void Set_Dealing_DamageOverTime_Timer();
	UFUNCTION() void Dealing_DamageOverTime(); // timer function
	UFUNCTION(BlueprintCallable) void Clear_Dealing_DamageOverTime_Timer();
	UFUNCTION(BlueprintCallable) void Calculate_CapsuleTraceShapes();
	UFUNCTION(BlueprintCallable) void Apply_FireDamage(AActor* damage_actor);

	////////////~ Begin IAbilitySystemInterface
	// Returns our Ability System Component. 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////////~ End IAbilitySystemInterface

	FVector Start_Loc;
	FVector End_Loc;
	
	//TArray <AActor*> Overlapping_Actors;

	FTimerHandle Dealing_DamageOverTime_TimerHandle;
	FTimerDelegate Dealing_DamageOverTime_Delegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf<UGameplayEffect> GE_Damage;
	
	//Use self-created asc if owner does'nt exist
	UPROPERTY(BlueprintReadWrite) UAbilitySystemComponent* Owner_ASC;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Ignore_Owner = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_TraceIgnoreSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Capsule_HiddenInGame = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Flame_Trace_Debug = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Trace_Interval = 0.1f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Radius = 30.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Length = 200.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Fire_Damage = 1.f;

	UPROPERTY(BlueprintReadOnly) AActor* Owner_A = nullptr; //If this Actor was created by a Child Actor Component returns the Actor that owns that Child Actor Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) TArray<AActor*> Trace_Ignore_Actors;
	
	/////////////////////Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UCapsuleComponent * Flame_Trigger_Capsule;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent * Start_Point;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent * End_Point;


protected:

	// ConstructionScript
	void OnConstruction(const FTransform &Transform) override;

	// Called when the game starts
	virtual void BeginPlay() override;


};
