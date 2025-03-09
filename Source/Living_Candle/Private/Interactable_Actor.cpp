// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Actor.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
AInteractable_Actor::AInteractable_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AInteractable_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}
//------------------------------------------------------------------------------------------------------------
//It is ñalled when an actor interacts with an object of this class. The Interact method must be overridden in child classes.
void AInteractable_Actor::Interact(AActor* actor)
{
	

}
//------------------------------------------------------------------------------------------------------------
// Broadcast, Call OnInteract delegate
void AInteractable_Actor::Call_OnInteract()
{
	On_Interact.Broadcast();

}
//------------------------------------------------------------------------------------------------------------
