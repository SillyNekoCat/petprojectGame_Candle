// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAble_Item.h"
#include "Wax_System_Comp.h"

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
void APickupAble_Item::Interact(AActor* actor)
{
	UWax_System_Comp* wax_system = actor->GetComponentByClass<UWax_System_Comp>();

	if(!IsValid(wax_system))
		return;

	switch(Item_Data.Item_Type)
	{
	case Enum_Pickupable_Item::EItem_Wick: 

		wax_system->Update_Wick_Items(Item_Data.Amount);
		break;
	case Enum_Pickupable_Item::EItem_Wax: 

		wax_system->Heal_Or_UpdateWaxItems(Item_Data.Amount);
		break;
	default: 
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("pickup type is undefined."));
		break;
	}


	On_Interact.Broadcast();

	Destroy();
}
//------------------------------------------------------------------------------------------------------------
