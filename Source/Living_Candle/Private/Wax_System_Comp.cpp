// Fill out your copyright notice in the Description page of Project Settings.


#include "Wax_System_Comp.h"
#include "AbilitySystemBlueprintLibrary.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UWax_System_Comp::UWax_System_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UWax_System_Comp::BeginPlay()
{
	Super::BeginPlay();
	

	//
	WickBurn_Delegate.BindUFunction(this, TEXT("Wick_Burn"));

	Owner = GetOwner();


	//
	Owner_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	Owner_BaseAttributeSet = Owner_ASC->GetSet<UBase_AttributeSet>(); 
	

	GE_Spec_Heal = *Owner_ASC->MakeOutgoingSpec(Heal_GE, 0, Owner_ASC->MakeEffectContext()).Data.Get();


	GE_Spec_Increase_HealthMax = *Owner_ASC->MakeOutgoingSpec(Increase_HealthMax_GE, 0, Owner_ASC->MakeEffectContext()).Data.Get();


	GE_Spec_Decrease_HealthMax = *Owner_ASC->MakeOutgoingSpec(Decrease_HealthMax_GE, 0, Owner_ASC->MakeEffectContext()).Data.Get();



}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UWax_System_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
//Timer Function, decreases Health_Max and call hud update
void UWax_System_Comp::Wick_Burn()
{

	Wick = Wick - Wick_Burn_Loss;
	Calculate_Max_HP();
	Check_Wick();

	Update_Wick_Bar();
	Update_Wax_Bar();

	Check_Wick();
}
//------------------------------------------------------------------------------------------------------------
//If Health + Wax_Wick_Ratio <= Health_Max, so then set Wick_Burn burn timer, otherwise clear timer.
void UWax_System_Comp::Check_Wick()
{
	
	if(Owner_BaseAttributeSet->GetHealth() + Wax_Wick_Ratio <= Owner_BaseAttributeSet->GetHealth_Max())
	{
		if (WickBurnTimer_Handle.IsValid())
		{	}
		else
		{
			Owner->GetWorldTimerManager().SetTimer(WickBurnTimer_Handle, WickBurn_Delegate, Wick_Burn_Interval, true, Wick_Burn_Interval);
		}

	}
	else
	{
		Owner->GetWorldTimerManager().ClearTimer(WickBurnTimer_Handle);
		//WickBurnTimer_Handle.Invalidate();//
		
	}
}
//------------------------------------------------------------------------------------------------------------
//
void UWax_System_Comp::Calculate_Max_HP()
{
	float new_max = Wick * Wax_Wick_Ratio;
	 

	if (Owner_BaseAttributeSet->GetHealth_Max() < new_max)
	{
		
		GE_Spec_Increase_HealthMax.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Health.Increase_HealthMax")), new_max - Owner_BaseAttributeSet->GetHealth_Max());
		Owner_ASC->ApplyGameplayEffectSpecToSelf(GE_Spec_Increase_HealthMax);
	}
	else
	{
		GE_Spec_Decrease_HealthMax.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Health.Decrease_HealthMax")), new_max - Owner_BaseAttributeSet->GetHealth_Max());
		Owner_ASC->ApplyGameplayEffectSpecToSelf(GE_Spec_Decrease_HealthMax);
	}


}
//------------------------------------------------------------------------------------------------------------
//Update HUD number using Wick_Items
void UWax_System_Comp::Update_Wick_Items(float amount)
{
	Wick_Items = Wick_Items + amount;
	Owner_PlayerHUD->Wick_Items = Wick_Items;

	Check_Can_Increase_MaxHP();
}
//------------------------------------------------------------------------------------------------------------
//Update HUD number using Wax_Items
void UWax_System_Comp::Update_Wax_Items(float amount)
{
	Wax_Items = Wax_Items + amount;
	Owner_PlayerHUD->Wax_Items = Wax_Items;
	
	Check_Can_Increase_MaxHP();
}
//------------------------------------------------------------------------------------------------------------
//Update HUD BAR using Wax_Items
void UWax_System_Comp::Update_Wax_Bar()
{
	Owner_PlayerHUD->Percent_Wax_Bar = Owner_BaseAttributeSet->GetHealth() / Owner_BaseAttributeSet->GetHealth_Max();
	Owner_PlayerHUD->Wax_Value_Text = Owner_BaseAttributeSet->GetHealth();
}
//------------------------------------------------------------------------------------------------------------
//Update HUD BAR using Wick_Items
void UWax_System_Comp::Update_Wick_Bar()
{
	Owner_PlayerHUD->Percent_Wick_Bar = (Owner_BaseAttributeSet->GetHealth_Max() / Wax_Wick_Ratio) / Wick;
	Owner_PlayerHUD->Wick_Value_Text = Wick;
}
//------------------------------------------------------------------------------------------------------------
//health(wax) system
void UWax_System_Comp::Check_Can_Increase_MaxHP()
{
	int result_wax_item, result_wick_item, wick_increase = 0;
	//Check can increase max hp
	if (Wax_Items / Wax_Wick_Ratio >= 1.f && Wick_Items > 0)
	{
		//Calculate remainder of items
		result_wax_item = FMath::Max( Wax_Items - (Wax_Wick_Ratio * Wick_Items), 0);
		result_wick_item = FMath::Max( Wick_Items - (Wax_Items / Wax_Wick_Ratio), 0);

		//Calculate increased wick, max_hp, current_hp and apply new values
		wick_increase = (Wick_Items - result_wick_item);
		Wick = Wick + wick_increase;

		Calculate_Max_HP();
		
		GE_Spec_Heal.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Health.Heal")), wick_increase * Wax_Wick_Ratio);
		Owner_ASC->ApplyGameplayEffectSpecToSelf(GE_Spec_Heal);

		//Apply remaining items
		Wax_Items = result_wax_item;
		Wick_Items = result_wick_item;

		//Update_UI
		Update_Wick_Items();
		Update_Wax_Items();
		Update_Wick_Bar();
		Update_Wax_Bar();
	}

}
//------------------------------------------------------------------------------------------------------------
//If Current_HP < Max_HP chose heal, else add wax item.  health(wax) system
void UWax_System_Comp::Heal_Or_UpdateWaxItems(float amount)
{

	float possible_heal_amount = Owner_BaseAttributeSet->GetHealth_Max() - Owner_BaseAttributeSet->GetHealth();
	float amount_after_heal = amount - possible_heal_amount;
	
	if (possible_heal_amount >= amount)
	{
		Heal(amount); 
	}
	else if(possible_heal_amount < amount)
	{
		Heal(possible_heal_amount);
		Update_Wax_Items(amount_after_heal);
	}
	
}
//------------------------------------------------------------------------------------------------------------
//
void UWax_System_Comp::Heal(float amount)
{
	Calculate_Max_HP();
	
	
	GE_Spec_Heal.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Health.Heal")), amount);
	Owner_ASC->ApplyGameplayEffectSpecToSelf(GE_Spec_Heal);
	
	Update_Wax_Bar();
}
//------------------------------------------------------------------------------------------------------------