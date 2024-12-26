// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage_System_Comp.h"
//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UDamage_System_Comp::UDamage_System_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UDamage_System_Comp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner()->Implements<UDamage_Interface>())
	{//Cast<IDamage_Interface>(GetOwner())->Take_Damage(this, damage_info, was_damage_applyed);
		Owner_DamageInterface = Cast<IDamage_Interface>(GetOwner());

	}
}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UDamage_System_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
//Call Take_Damage interface function in owner of this component
void UDamage_System_Comp::Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged)
{
	//GetOwner()->Take_Damage(damage_causer, damage_info, was_damaged);

	if (GetOwner()->Implements<UDamage_Interface>())
	{
		
		Owner_DamageInterface->Take_Damage(damage_causer, damage_info, was_damaged);
	}
	
}
//------------------------------------------------------------------------------------------------------------
