// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Base_AttributeSet.h" 
#include "GameFramework/Character.h"
#include "NPC.generated.h"
class UBase_AttributeSet;
class UAbilitySystemComponent;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API ANPC : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	////////////~ Begin IAbilitySystemInterface
	// Returns our Ability System Component. 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	////////////~ End IAbilitySystemInterface

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Max_HP = 1.0f;//DELETE
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Current_HP = 1.0f;//DELETE

	////////////Components
	UPROPERTY(BlueprintReadWrite) const UBase_AttributeSet* Base_AttributeSet;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Abilities") UAbilitySystemComponent* AbilitySystem_Comp;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) UWidgetComponent* Widget_Component;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
