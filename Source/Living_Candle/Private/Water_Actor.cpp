// Fill out your copyright notice in the Description page of Project Settings.


#include "Water_Actor.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
AWater_Actor::AWater_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AWater_Actor::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void AWater_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
