// Fill out your copyright notice in the Description page of Project Settings.


#include "Targets_Manager.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UTargets_Manager::UTargets_Manager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UTargets_Manager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UTargets_Manager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
//
void UTargets_Manager::Send_Targets(TArray <UAbilitySystemComponent*> targets_ability_system_comp)
{
	for (UAbilitySystemComponent* target_asc : targets_ability_system_comp)
	{
		On_SendTargets.Broadcast(target_asc);
	}
	
}
//------------------------------------------------------------------------------------------------------------