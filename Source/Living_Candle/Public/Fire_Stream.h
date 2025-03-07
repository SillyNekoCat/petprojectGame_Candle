// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interact_CapsuleComponent.h"
#include "Interact_BoxComponent.h"
#include "Interact_SphereComponent.h"
#include "Fire_Stream.generated.h"
class UAbilitySystemComponent;
class UInteract_CapsuleComponent;
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

	UFUNCTION(BlueprintCallable) void Set_Dealing_DamageOverTime_Timer();
	UFUNCTION() void Dealing_DamageOverTime(); // timer function
	UFUNCTION(BlueprintCallable) void Clear_Dealing_DamageOverTime_Timer();
	UFUNCTION(BlueprintCallable) void Calculate_CapsuleTraceShapes();

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Ignore_Owner = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Is_TraceIgnoreSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Is_Capsule_HiddenInGame = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Flame_Trace_Debug = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) float Trace_Interval = 0.1f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) float Radius = 30.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) double Length = 200.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) float Fire_Damage = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = FireStream) AActor* Owner_A = nullptr; //If this Actor was created by a Child Actor Component returns the Actor that owns that Child Actor Component
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) TArray<AActor*> Trace_Ignore_Actors;
	
	/////////////////////Components
	UPROPERTY(BlueprintReadWrite, Category = FireStream) UInteract_CapsuleComponent * Capsule;
	UPROPERTY(BlueprintReadWrite, Category = FireStream) USceneComponent * Start_Point;
	UPROPERTY(BlueprintReadWrite, Category = FireStream) USceneComponent * End_Point;


protected:

	// ConstructionScript
	void OnConstruction(const FTransform &Transform) override;

	// Called when the game starts
	virtual void BeginPlay() override;


};
