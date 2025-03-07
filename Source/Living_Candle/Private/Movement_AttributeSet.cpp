// Fill out your copyright notice in the Description page of Project Settings.


#include "Movement_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/CharacterMovementComponent.h"

//------------------------------------------------------------------------------------------------------------
//
bool UMovement_AttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	Super::PreGameplayEffectExecute(Data);

	return true;
}
//------------------------------------------------------------------------------------------------------------
//
void UMovement_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);



}
//------------------------------------------------------------------------------------------------------------
//
void UMovement_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

}
//------------------------------------------------------------------------------------------------------------
//
void UMovement_AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMove_SpeedAttribute())
	{
		GetOwningActor()->GetComponentByClass<UCharacterMovementComponent>()->MaxWalkSpeed = NewValue;
	}

}
//------------------------------------------------------------------------------------------------------------

