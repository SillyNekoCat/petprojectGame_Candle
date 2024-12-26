// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericStructs.h"
#include "Heat_Component.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIVING_CANDLE_API UHeat_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeat_Component();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION() void Cooling_Timer();
	UFUNCTION() void Retriggerable_Cooling_Delay();                    

	UFUNCTION() void Calculate_HeatContactDamage(AActor *target, double &contact_damage);
	
	

	UFUNCTION() void HeatDamage_Take(FDamage_Inf damage_inf, bool was_owner_damaged);//Call from delegate when we take fire damage
	void Heat_Lose(double damage);

	UFUNCTION(BlueprintCallable) void Calculate_MaxHeat();

	double Max_Accumulated_Heat;
	AActor* Damage_Causer;
	FTimerHandle CoolingTimer_Handle;
	FTimerHandle RetriggCoolingDelay_Handle;
	FTimerDelegate Cooling_Delegate;
	FTimerDelegate RetriggCoolingDelay_Delegate;
	UMaterialInstanceDynamic *Mat_Heat_Inst = 0;
	//UPROPERTY(BlueprintReadWrite) TArray<AActor*> Overlapped_Actors;

	//Is_Give_PartOfMaxHeat
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Give_PartOfMaxHeat = false;
	//DamageIfTarget_NotHave_HeatComp
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool DamageIfTarget_NotHave_HeatComp = true;
	//DamageIfTarget_NotHave_HeatComp
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool DamageIfTarget_Have_HeatComp = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Can_Heat = true;
	//UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Can_KnockBack = false;

	//(0-1) using as parameter in material instance, 1 means that it has maximum heating
	UPROPERTY(BlueprintReadOnly) double Heat_Status_Param = 0.0;
	//its value means accumulated fire damage, it falls down with Heat_Status_Param
	UPROPERTY(BlueprintReadOnly) double Accumulated_Heat = 0.0;

	//Give_PartOfHeat_HaveHeatComp Value is 0 - 1 (1 value will give all heat)
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Give_PartOfHeat_HaveHeatComp = 0.5;
	//Give_PartOfHeat_NotHaveHeatComp Value is 0 - 1 (1 value will give all heat)
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Give_PartOfHeat_NotHaveHeatComp = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Heat_Status_Start = 0.0;
	//the higher value, the slowly it will heat up and higher maximum accumulated heat will have(если делители разные то нагрев будет неправильно делится)
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double HeatResistance_Divider = 5.0;

	//the higher value, the faster it will cooling, use a fractional percentage value for example: (0-1) 0.1 = 10%, 1 = 100%
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Cooling_Percent_Loss = 0.1;
	//the higher value, the slowly it will cooling
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Cooling_Timer_Interval = 0.2;
	//It will be infinity if value < 0
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Cooling_Setup_Delay = 1.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UMaterialInterface* Parent_Mat_Heat = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<TSubclassOf<UObject>> Ignore_Actors;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
