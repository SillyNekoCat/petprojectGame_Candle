// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"//
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
{//Can it capture more than one magnitude at the time?
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayEffectContextHandle& effect_context = Data.EffectSpec.GetEffectContext();
	AActor* instigator = effect_context.GetOriginalInstigator();
	AActor* causer = effect_context.GetEffectCauser();
	float damage = 0.0f;
	
	FGameplayTag damage_tag;
	
	//FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys"))
	//FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire"))
	//FGameplayTag::RequestGameplayTag(FName("DamageTypes.Pure"))
	
	//Incoming_Phys_Damage
	if (Data.EvaluatedData.Attribute == GetIncoming_Phys_DamageAttribute())
	{
		damage_tag = FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys"));
		damage = GetIncoming_Phys_Damage();

		if (Can_Change_Health)
		{
			
			SetHealth(FMath::Clamp(GetHealth() - damage, 0.0f, GetHealth_Max()));
			SetIncoming_Phys_Damage(0.0f);
		}
	}

	//Incoming_Fire_Damage
	if (Data.EvaluatedData.Attribute == GetIncoming_Fire_DamageAttribute())
	{
		damage_tag = FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire"));
		damage = GetIncoming_Fire_Damage();

		if (Can_Change_Health)
		{
			SetHealth(FMath::Clamp(GetHealth() - damage, 0.0f, GetHealth_Max()));
			SetIncoming_Fire_Damage(0.0f);
		}
	}

	//Incoming_Pure_Damage
	if (Data.EvaluatedData.Attribute == GetIncoming_Pure_DamageAttribute())
	{
		damage_tag = FGameplayTag::RequestGameplayTag(FName("DamageTypes.Pure"));
		damage = GetIncoming_Pure_Damage();

		if (Can_Change_Health)
		{ 
			SetHealth(FMath::Clamp(GetHealth() - damage, 0.0f, GetHealth_Max()));
			SetIncoming_Pure_Damage(0.0f);
		}

	}
	
	//for (FGameplayEffectModifiedAttribute modified_attr : Data.EffectSpec.ModifiedAttributes) 
	
	On_Damage_Take.Broadcast(instigator, causer, damage_tag, damage, HealthBeforeAttributeChange, GetHealth());

	//On Health changed
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		//On_Health_Changed.Broadcast(instigator, causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
		
		On_Health_Changed.Broadcast(instigator, causer, damage_tag, damage, HealthBeforeAttributeChange, GetHealth());

		//On Zero Health (death?)
		if(GetHealth() <= 0.0f)
			On_Health_Zero.Broadcast(instigator, causer, damage_tag, damage, HealthBeforeAttributeChange, GetHealth());
	}
	else //On Damage Effect recieved but health not changed
	{
		

	}

}
//------------------------------------------------------------------------------------------------------------
