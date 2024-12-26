// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Water_Actor.generated.h"

UCLASS()
class LIVING_CANDLE_API AWater_Actor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AWater_Actor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
