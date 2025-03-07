// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAble_Item.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
APickupAble_Item::APickupAble_Item()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Pickup_Sphere = CreateDefaultSubobject<UInteract_SphereComponent>(TEXT("Pickup_Sphere"));
	Pickup_Sphere->SetupAttachment(RootComponent);
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void APickupAble_Item::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
// 
void APickupAble_Item::Pickup(FPickupableItem_Data& data)
{
	data = Item_Data;

	Destroy();
}
//------------------------------------------------------------------------------------------------------------
