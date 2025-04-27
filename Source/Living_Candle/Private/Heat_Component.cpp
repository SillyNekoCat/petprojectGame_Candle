// Fill out your copyright notice in the Description page of Project Settings.


#include "Heat_Component.h"
#include "Melee_Weapon.h"
#include "Kismet/KismetMathLibrary.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UHeat_Component::UHeat_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Bind timer functions
	Cooling_Delegate.BindUFunction(this, TEXT("Cooling_Timer"));
	RetriggCoolingDelay_Delegate.BindUFunction(this, TEXT("Retriggerable_Cooling_Delay"));

}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UHeat_Component::BeginPlay()
{
	Super::BeginPlay();
	
	////Set overlay material for all meshes
	if (Can_Heat == true)
	{
		Calculate_MaxHeat();

		if (Parent_Mat_Heat != nullptr)
		{//Create Material Inst for all meshes in blueprint hierarchy

			TArray<UActorComponent*> comps;
			GetOwner()->GetComponents(comps);//Gain array of components

			Mat_Heat_Inst = UMaterialInstanceDynamic::Create(Parent_Mat_Heat, this);

			for (int i = 0; i < comps.Num(); i++)
			{//Set Material Inst for all meshes
				if (UMeshComponent* mesh_comp = Cast<UMeshComponent>(comps[i]))
					mesh_comp->SetOverlayMaterial(Mat_Heat_Inst);
			}
		}
	}

	Set_Heat_Status_Param(Heat_Status_Start);//////

	if(Mat_Heat_Inst != nullptr)
		Mat_Heat_Inst->SetScalarParameterValue(FName("HeatOpacity"), Heat_Status_Param);

	GetOwner()->GetWorldTimerManager().SetTimer(RetriggCoolingDelay_Handle, RetriggCoolingDelay_Delegate, Cooling_Setup_Delay, false, -1.0f);

}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UHeat_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
//Looping Function
void UHeat_Component::Cooling_Timer()
{

	if (Heat_Status_Param <= 0.0)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(CoolingTimer_Handle);
	}

	Set_Heat_Status_Param(Heat_Status_Param - Cooling_Percent_Loss);////////////

	if(Mat_Heat_Inst != nullptr)
		Mat_Heat_Inst->SetScalarParameterValue(FName("HeatOpacity"), Heat_Status_Param);

	if (Heat_Status_Param <= 0.0)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(CoolingTimer_Handle);
	}


}
//------------------------------------------------------------------------------------------------------------
//After delay starts Cooling_Timer, the delay is retriggerable by fire damage
void UHeat_Component::Retriggerable_Cooling_Delay()
{
	GetOwner()->GetWorldTimerManager().SetTimer(CoolingTimer_Handle, Cooling_Delegate, Cooling_Timer_Interval, true, -1.0f);
}
//------------------------------------------------------------------------------------------------------------
//
double UHeat_Component::Calculate_HeatContactDamage(AActor *target)
{

	Last_FireContactDamage = 0;

		  
		
	if (Ignore_Actors.Contains(target->StaticClass()) || Cast<AMelee_Weapon>(target) )
	{
		return Last_FireContactDamage;
	}
	

	if (UHeat_Component* same_comp = Cast<UHeat_Component>(target->GetComponentByClass(UHeat_Component::StaticClass()) ) )
	{
		if(DamageIfTarget_Have_HeatComp == true)
		{
			if (same_comp->Heat_Status_Param < Heat_Status_Param)
			{


				Last_FireContactDamage = (Accumulated_Heat - same_comp->Accumulated_Heat) * Give_PartOfHeat_HaveHeatComp;
				
				//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, FString::Printf(TEXT("Last_FireContactDamage = %f"), Last_FireContactDamage));//temp
				
				Heat_Lose(Last_FireContactDamage);
				//return Last_FireContactDamage;
				

			}
		}
	}
	else if(DamageIfTarget_NotHave_HeatComp == true)
	{
		if (Is_Give_PartOfMaxHeat == true)//test
		{
			if (Accumulated_Heat <= Max_Accumulated_Heat * Give_PartOfHeat_NotHaveHeatComp)
			{
				Last_FireContactDamage = Accumulated_Heat;
			}
			else
			{
				Last_FireContactDamage =  Max_Accumulated_Heat * Give_PartOfHeat_NotHaveHeatComp;
			}
		}
		else 
		{
			Last_FireContactDamage = Accumulated_Heat * Give_PartOfHeat_NotHaveHeatComp;
		}
		
		Heat_Lose(Last_FireContactDamage); 

	}
	
	//DEBUG
	if(Debug)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("UHeat_Component::Calculate_HeatContactDamage Last_FireContactDamage = %f"), Last_FireContactDamage));//debug temp
	
	
	
	return Last_FireContactDamage;

}
//------------------------------------------------------------------------------------------------------------
//Call when owner take fire damage
void UHeat_Component::HeatDamage_Take(AActor* EffectInstigator, AActor* EffectCauser, FDamage_Info damage_info)
{
	if(!Can_Heat)
		return;

	
	//if( !DamageTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire"))) )
	//	return;

	if(damage_info.Fire_Damage == 0.0)
		return;



	GetOwner()->GetWorldTimerManager().ClearTimer(CoolingTimer_Handle);


	Set_Accumulated_Heat(Accumulated_Heat + damage_info.Fire_Damage);////////

	if(Mat_Heat_Inst != nullptr)
		Mat_Heat_Inst->SetScalarParameterValue(FName("HeatOpacity"), Heat_Status_Param);


	//
	GetOwner()->GetWorldTimerManager().SetTimer(RetriggCoolingDelay_Handle, RetriggCoolingDelay_Delegate, Cooling_Setup_Delay, false, -1.0f);
}
//------------------------------------------------------------------------------------------------------------
// 
void UHeat_Component::Heat_Lose(double damage)
{

	Set_Accumulated_Heat(Accumulated_Heat - damage);////////

	if(Mat_Heat_Inst != nullptr)
		Mat_Heat_Inst->SetScalarParameterValue(FName("HeatOpacity"), Heat_Status_Param);

}
//------------------------------------------------------------------------------------------------------------
//
void UHeat_Component::Calculate_MaxHeat()
{
	Max_Accumulated_Heat = Heat_Divider * 1.0;
}
//------------------------------------------------------------------------------------------------------------
//
void UHeat_Component::Set_Accumulated_Heat(double value)
{
	Accumulated_Heat = FMath::Clamp(value, 0, Max_Accumulated_Heat);
	Set_Heat_Status_Param(Accumulated_Heat / Heat_Divider);
}
//------------------------------------------------------------------------------------------------------------
//
void UHeat_Component::Set_Heat_Status_Param(double value)
{
	
	Heat_Status_Param = FMath::Clamp(value, 0.0, 1.0);

	Accumulated_Heat = Heat_Status_Param * Heat_Divider;

	if(Mat_Heat_Inst != nullptr)
		Mat_Heat_Inst->SetScalarParameterValue(FName("HeatOpacity"), Heat_Status_Param);

	OnChangeHeat_Delegate.Broadcast(Heat_Status_Param);
}