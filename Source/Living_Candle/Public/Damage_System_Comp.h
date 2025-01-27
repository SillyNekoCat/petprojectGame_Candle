// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Damage_System_Comp.generated.h"
//------------------------------------------------------------------------------------------------------------
//This component helps with using c++ interface and other tasks...
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable)
class LIVING_CANDLE_API UDamage_System_Comp : public UActorComponent
{//This component helps with using c++ interface and other tasks... Это компонент для взаимодействия с С++ интерфейсом(он теперь удален)
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamage_System_Comp();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//virtual void Get_Current_Health(double &health);
	//virtual void Get_Max_Health(double &max_health);
	//virtual void Heal(double heal_amount, double &new_health);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;



		
};
//------------------------------------------------------------------------------------------------------------
