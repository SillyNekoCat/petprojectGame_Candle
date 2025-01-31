// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHUD.h"
#include "AbilitySystemComponent.h"
#include "Base_AttributeSet.h" 
#include "Components/ActorComponent.h"
#include "Wax_System_Comp.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable)
class LIVING_CANDLE_API UWax_System_Comp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWax_System_Comp();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Repeating timer function, need for health(wax) system
	UFUNCTION() void Wick_Burn();
	//health(wax) system
	UFUNCTION(BlueprintCallable) void Check_Wick();
	//health(wax) system
	UFUNCTION(BlueprintCallable) void Calculate_Max_HP();
	//Update HUD number using Wick_Items
	UFUNCTION(BlueprintCallable) void Update_Wick_Items(float amount = 0.f);
	//Update HUD number using Wax_Items
	UFUNCTION(BlueprintCallable) void Update_Wax_Items(float amount = 0.f);
	//Update HUD BAR using Wax_Items
	UFUNCTION(BlueprintCallable) void Update_Wax_Bar();
	//Update HUD BAR using Wick_Items
	UFUNCTION(BlueprintCallable) void Update_Wick_Bar();
	//health(wax) system
	UFUNCTION(BlueprintCallable) void Check_Can_Increase_MaxHP();
	
	UFUNCTION(BlueprintCallable) void Heal_Or_UpdateWaxItems(float amount = 0.f);

	UFUNCTION(BlueprintCallable) void Heal(float amount = 0.f);

	FTimerHandle WickBurnTimer_Handle;
	FTimerDelegate WickBurn_Delegate;


	UPROPERTY(BlueprintReadWrite) FGameplayEffectSpec GE_Spec_Heal;
	UPROPERTY(BlueprintReadWrite) FGameplayEffectSpec GE_Spec_Increase_HealthMax;
	UPROPERTY(BlueprintReadWrite) FGameplayEffectSpec GE_Spec_Decrease_HealthMax;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf<UGameplayEffect> Heal_GE;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf<UGameplayEffect> Increase_HealthMax_GE;//Health.Increase_HealthMax
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TSubclassOf<UGameplayEffect> Decrease_HealthMax_GE;//Health.Decrease_HealthMax

	UPROPERTY(BlueprintReadOnly) const UBase_AttributeSet* Owner_BaseAttributeSet;
	UPROPERTY(BlueprintReadOnly) UAbilitySystemComponent* Owner_ASC;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Max_HP = 1.0f;//переносить в AttributeSet
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Current_HP = 1.0f;//переносить в AttributeSet
	
	UPROPERTY(BlueprintReadWrite) int Wick_Items = 0; //Стоит ли переносить в AttributeSet?
	UPROPERTY(BlueprintReadWrite) int Wax_Items = 0; //Стоит ли переносить в AttributeSet?

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Wick = 4.0f; //Стоит ли переносить в AttributeSet?
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Wax_Wick_Ratio = 5.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Wick_Burn_Loss = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Wick_Burn_Interval = 2.0f;
	
	UPROPERTY(BlueprintReadOnly) AActor* Owner = nullptr;
	UPROPERTY(BlueprintReadWrite) UPlayerHUD* Owner_PlayerHUD;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
//------------------------------------------------------------------------------------------------------------
