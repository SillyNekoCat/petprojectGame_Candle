// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damage_Interface.h"
#include "Components/ActorComponent.h"
#include "Damage_System_Comp.generated.h"
//------------------------------------------------------------------------------------------------------------
//This component helps with using c++ interface and other tasks...
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIVING_CANDLE_API UDamage_System_Comp : public UActorComponent
{//This component helps with using c++ interface and other tasks... Это компонент для взаимодействия с С++ интерфейсом
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamage_System_Comp();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//virtual void Get_Current_Health(double &health);
	//virtual void Get_Max_Health(double &max_health);
	//virtual void Heal(double heal_amount, double &new_health);
	//virtual void Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged);

	//Handling and calculate incoming damage specific to all owners, after that call Take_Damage interface function in owner of this component
	UFUNCTION(BlueprintCallable) void Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged);

	/*UPROPERTY(BlueprintReadWrite)*/ IDamage_Interface* Owner_DamageInterface;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



		
};
//------------------------------------------------------------------------------------------------------------
