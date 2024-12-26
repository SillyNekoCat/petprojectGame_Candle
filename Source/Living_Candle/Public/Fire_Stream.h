// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "Damage_Interface.h"
#include "GameFramework/Actor.h" 
#include "Interact_CapsuleComponent.h"
#include "Interact_BoxComponent.h"
#include "Interact_SphereComponent.h"
#include "Fire_Stream.generated.h"

class UInteract_CapsuleComponent;
class USceneComponent;

UCLASS()
class LIVING_CANDLE_API AFire_Stream : public AActor//, public IDamage_Interface
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
	UFUNCTION(BlueprintCallable) void Revert_FlameStreamDamage();
	UFUNCTION(BlueprintCallable) void Modify_FlameStreamDamage(double phys = 0.0, double fire = 0.0, double knockback = 0.0, double stun = 0.0);
	UFUNCTION(BlueprintCallable) void Calculate_CapsuleTraceShapes();


	FVector Start_Loc;
	FVector End_Loc;
	
	TArray <AActor*> Overlapping_Actors;

	FTimerHandle Dealing_DamageOverTime_TimerHandle;
	FTimerDelegate Dealing_DamageOverTime_Delegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Ignore_Owner = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Is_TraceIgnoreSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Is_Capsule_HiddenInGame = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) bool Flame_Trace_Debug = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) float Trace_Interval = 0.1f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) float Radius = 30.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) double Length = 200.0;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) FDamage_Inf FlameStream_BaseDamage_Info;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) FDamage_Inf FlameStream_CurrentDamage_Info;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FireStream) AActor* Owner_A = nullptr;
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
