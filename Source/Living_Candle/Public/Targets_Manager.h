// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Targets_Manager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendTargets_Delegate, UAbilitySystemComponent*, asc_to_damage);

//Every object that getting targets, for example from trace, must have his own Target_Manager component. I made this because a dont know how apply gameplay effects in C++
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIVING_CANDLE_API UTargets_Manager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargets_Manager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void Send_Targets(TArray <UAbilitySystemComponent*> targets_ability_system_comp);

	UPROPERTY(BlueprintAssignable) FSendTargets_Delegate On_SendTargets;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
