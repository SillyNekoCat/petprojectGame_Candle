// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack_Comp.h"
//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UAttack_Comp::UAttack_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UAttack_Comp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UAttack_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
// 
void UAttack_Comp::OnMaxCombo_Reset_Index()
{
	if(Attack_Index >= SerialAttack_Montages.Num())
		Attack_Index = 0;
}
//------------------------------------------------------------------------------------------------------------
// 
void UAttack_Comp::NotifyState_Tracing_Attack()
{
	if (Current_Weapon != nullptr)
	{
		Current_Weapon->Attack_Trace();
	}
	else//for example empty hands
	{

	}
		
}
//------------------------------------------------------------------------------------------------------------
