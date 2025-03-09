// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable_Actor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract_Delegate);
UCLASS()
class LIVING_CANDLE_API AInteractable_Actor : public AActor
{//—корее всего этот класс костыль
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AInteractable_Actor();

	//It is сalled when an actor interacts with an object of this class. The Interact method must be overridden in child classes.
	UFUNCTION(BlueprintCallable) virtual void Interact(AActor* actor);

	UFUNCTION(BlueprintCallable) void Call_OnInteract();

	UPROPERTY(BlueprintAssignable) FOnInteract_Delegate On_Interact;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
