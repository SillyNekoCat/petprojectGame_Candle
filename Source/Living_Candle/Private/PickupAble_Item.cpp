// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAble_Item.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
APickupAble_Item::APickupAble_Item()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
// Called every frame
void APickupAble_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
// 
void APickupAble_Item::Pickup(Enum_Pickupable_Item & pickupable_item_type, float & amount)
{
	pickupable_item_type = Pickupable_Item_Type;
	amount = Amount;

	Destroy();
}
//------------------------------------------------------------------------------------------------------------
