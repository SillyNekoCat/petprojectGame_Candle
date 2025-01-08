// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AttributeSet.h"
#include "GameplayEffectExtension.h"

//------------------------------------------------------------------------------------------------------------
//
UBase_AttributeSet::UBase_AttributeSet()
{
	HealthBeforeAttributeChange = 0.f;
	MaxHealthBeforeAttributeChange = 0.f;

}
//------------------------------------------------------------------------------------------------------------
//
bool UBase_AttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	Super::PreGameplayEffectExecute(Data);

	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetHealth_Max();

	return true;
}
//------------------------------------------------------------------------------------------------------------
//
void UBase_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	
	FDamage_Inf damage_inf;

	//Incoming_Phys_Damage
	//if (Data.EvaluatedData.Attribute == GetIncoming_Phys_DamageAttribute()) ////это условие не пропускало почему-то
	//{
		damage_inf.Phys_Damage = GetIncoming_Phys_Damage();
		if (Can_Change_Health)
		{
			SetHealth(FMath::Clamp(GetHealth() - GetIncoming_Phys_Damage(), 0.0f, GetHealth_Max()));
			SetIncoming_Phys_Damage(0.0f);
		}
	//}

	//Incoming_Fire_Damage
	//if (Data.EvaluatedData.Attribute == GetIncoming_Fire_DamageAttribute())
	//{
		damage_inf.Fire_Damage = GetIncoming_Fire_Damage();
		if (Can_Change_Health)
		{
			SetHealth(FMath::Clamp(GetHealth() - GetIncoming_Fire_Damage(), 0.0f, GetHealth_Max()));
			SetIncoming_Fire_Damage(0.0f);
		}
	//}

	//Incoming_Pure_Damage
	//if (Data.EvaluatedData.Attribute == GetIncoming_Pure_DamageAttribute())
	//{
		damage_inf.Pure_Damage = GetIncoming_Pure_Damage();
		if (Can_Change_Health)
		{ 
			SetHealth(FMath::Clamp(GetHealth() - GetIncoming_Pure_Damage(), 0.0f, GetHealth_Max()));
			SetIncoming_Pure_Damage(0.0f);
		}
	//}
	
	On_Damage_Take;
	On_Health_Changed;
	On_Health_Zero;

	//Data.EffectSpec.ModifiedAttributes

	//On Damage recieved
	On_Damage_Take.Broadcast(Instigator, Causer, Data.EffectSpec, damage_inf, HealthBeforeAttributeChange, GetHealth());

	//On Health changed
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		//On_Health_Changed.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
		
		On_Health_Changed.Broadcast(Instigator, Causer, Data.EffectSpec, damage_inf, HealthBeforeAttributeChange, GetHealth());
	}

	if (GetHealth() != HealthBeforeAttributeChange)
	{
		On_Health_Zero.Broadcast(Instigator, Causer, Data.EffectSpec, damage_inf, HealthBeforeAttributeChange, GetHealth());
	}
}
//------------------------------------------------------------------------------------------------------------
