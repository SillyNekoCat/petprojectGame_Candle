// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Knockback_Comp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIVING_CANDLE_API UKnockback_Comp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKnockback_Comp();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable) FVector Impulse_Calculate(FVector start, FVector target);
	UFUNCTION(BlueprintCallable) void Knockback_Impulse(AActor* actor, UPrimitiveComponent* comp, AActor* owner_of_weapon);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Velocity_Change = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Impulse = 250.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) double Z_Impulse = 200.0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
