// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
* 
*/
UCLASS()
class LIVING_CANDLE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Widget to use to display current health. */
	//UPROPERTY(EditAnywhere, meta = (BindWidget)) class UProgressBar* WaxHealth_Bar;

	/** Widget to use to display current power. */
	//UPROPERTY(EditAnywhere, meta = (BindWidget)) class UProgressBar* WickHealth_Bar;

	//UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* Wax_Value;

	//UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* Wick_Value;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Percent_Wax_Bar = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Percent_Wick_Bar = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Wax_Value_Text = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Wick_Value_Text = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Wick_Items = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Wax_Items = 0;
};
