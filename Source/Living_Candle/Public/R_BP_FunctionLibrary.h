// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "R_BP_FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API UR_BP_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Obtain an Array of Actors Rendered Recently. You can specifiy what qualifies as "Recent" in seconds. */
	UFUNCTION(BlueprintCallable, Category = "UR_BP_FunctionLibrary|Actor", meta=(WorldContext="world_context_object"))
	static void Visibility__GetRenderedActors(UObject* world_context_object, TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime = 0.01);

	/** Obtain an Array of Actors NOT Rendered Recently! You can specifiy what qualifies as "Recent" in seconds. */
	UFUNCTION(BlueprintCallable, Category = "UR_BP_FunctionLibrary|Actor", meta=(WorldContext="world_context_object"))
	static void Visibility__GetNotRenderedActors(UObject* world_context_object, TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime = 0.01);

	/** Is_On_Screen */
	UFUNCTION(BlueprintCallable, Category = "UR_BP_FunctionLibrary|Actor", meta=(WorldContext="world_context_object"))
	static bool Is_On_Screen(UObject* world_context_object, AActor* target, APlayerController* player_controller);


};
