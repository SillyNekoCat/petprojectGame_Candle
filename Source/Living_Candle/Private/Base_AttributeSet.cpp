// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Attack_Comp.h"
//#include "Kismet/KismetMathLibrary.h"

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
	
	const FGameplayEffectContextHandle& effect_context = Data.EffectSpec.GetEffectContext();
	AActor* instigator = effect_context.GetOriginalInstigator();
	AActor* causer = effect_context.GetEffectCauser();
	float magnitude = 0.0f;
	
	FGameplayTag tag;

	//Heal
	if (Data.EvaluatedData.Attribute == GetIncoming_HealAttribute())
	{
		tag = FGameplayTag::RequestGameplayTag(FName("Health.Heal"));
		magnitude = GetIncoming_Heal();
		if (Can_Heal)
		{
			SetHealth(FMath::Clamp(GetHealth() + magnitude, 0.0f, GetHealth_Max()));
			SetIncoming_Heal(0.0f);
		}
	}

	//FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys"))
	//FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire"))
	//FGameplayTag::RequestGameplayTag(FName("DamageTypes.Pure"))
	
	

	//Incoming_Phys_Damage
	if (Data.EvaluatedData.Attribute == GetIncoming_Phys_DamageAttribute())
	{
		tag = FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys"));
		magnitude = GetIncoming_Phys_Damage();

		if (Can_Take_Damage)
		{
			
			SetHealth(FMath::Clamp(GetHealth() - magnitude, 0.0f, GetHealth_Max()));
			SetIncoming_Phys_Damage(0.0f);
		}
	}

	//Incoming_Fire_Damage
	if (Data.EvaluatedData.Attribute == GetIncoming_Fire_DamageAttribute())
	{
		tag = FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire"));
		magnitude = GetIncoming_Fire_Damage();

		if (Can_Take_Damage)
		{
			SetHealth(FMath::Clamp(GetHealth() - magnitude, 0.0f, GetHealth_Max()));
			SetIncoming_Fire_Damage(0.0f);
		}
	}

	//Incoming_Pure_Damage
	if (Data.EvaluatedData.Attribute == GetIncoming_Pure_DamageAttribute())
	{
		tag = FGameplayTag::RequestGameplayTag(FName("DamageTypes.Pure"));
		magnitude = GetIncoming_Pure_Damage();

		if (Can_Take_Damage)
		{ 
			SetHealth(FMath::Clamp(GetHealth() - magnitude, 0.0f, GetHealth_Max()));
			SetIncoming_Pure_Damage(0.0f);
		}

	}
	
	//for (FGameplayEffectModifiedAttribute modified_attr : Data.EffectSpec.ModifiedAttributes) 
	
	On_Damage_Take.Broadcast(instigator, causer, tag, magnitude, HealthBeforeAttributeChange, GetHealth());

	//On Health changed
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		//On_Health_Changed.Broadcast(instigator, causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
		
		On_Health_Changed.Broadcast(instigator, causer, tag, magnitude, HealthBeforeAttributeChange, GetHealth());

		//On Zero Health (death)
		if(GetHealth() <= 0.0f)
			On_Health_Zero.Broadcast(instigator, causer, tag, magnitude, HealthBeforeAttributeChange, GetHealth());
	}
	else //On Damage Effect recieved but health not changed
	{
		

	}

}
//------------------------------------------------------------------------------------------------------------
//
void UBase_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

}
//------------------------------------------------------------------------------------------------------------
//
void UBase_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

}
//------------------------------------------------------------------------------------------------------------
