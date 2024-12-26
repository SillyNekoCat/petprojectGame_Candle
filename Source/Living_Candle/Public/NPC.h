// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damage_Interface.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

class UWidgetComponent;

//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API ANPC : public ACharacter, public IDamage_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	////////////~ Begin IDamage_Interface
	//handling incoming damage specific to this class
	virtual void Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged) override;
	////////////~ End IDamage_Interface

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Max_HP = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Current_HP = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) UWidgetComponent* Widget_Component;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
