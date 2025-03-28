// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
//#include "GenericStructs.h"
#include "Base_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//DECLARE_MULTICAST_DELEGATE_SixParams(FBaseAttributeSet_Delegate, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FBaseAttributeSet_Delegate, AActor*, EffectInstigator, AActor*, EffectCauser, FGameplayTag, DamageTag, float, Magnitude, float, OldValue, float, NewValue);


/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API UBase_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBase_AttributeSet();

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue);
	
	void ActiveBlock(AActor* causer);
	
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	UPROPERTY(BlueprintAssignable) FBaseAttributeSet_Delegate On_Damage_Take;//Attribute event
	UPROPERTY(BlueprintAssignable) FBaseAttributeSet_Delegate On_Health_Changed;//Attribute event
	UPROPERTY(BlueprintAssignable) FBaseAttributeSet_Delegate On_Health_Zero;//Attribute event

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool Can_Take_Damage = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool Can_Heal = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health") FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health") FGameplayAttributeData Health_Max;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Health_Max)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Phys_Block = 0.f;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Phys_Block)

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Fire_Block = 0.f;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Fire_Block)



	// --------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below)
	// --------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Incoming_Phys_Damage = 0.f;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Incoming_Phys_Damage)

		UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Incoming_Fire_Damage = 0.f;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Incoming_Fire_Damage)

		UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Incoming_Pure_Damage = 0.f;
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Incoming_Pure_Damage)
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayAttributeData Incoming_Heal = 0.f;// не нужен?
	ATTRIBUTE_ACCESSORS(UBase_AttributeSet, Incoming_Heal)

};
