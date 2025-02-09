// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEC_KnockbackExecution.generated.h"

/**
 * 
 */
UCLASS()
class LIVING_CANDLE_API UGEC_KnockbackExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEC_KnockbackExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	FVector Impulse_Calculate(FVector start, FVector target, float impulse, float z_impulse) const;
	void Knockback_Impulse(AActor* actor, AActor* owner_of_weapon, float impulse) const;
};
