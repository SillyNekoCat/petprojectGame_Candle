// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"//Interactable_Actor является наследником Actor
#include "Interactable_Actor.h"
#include "Interact_SphereComponent.h"
#include "PickupAble_Item.generated.h"

class UInteract_SphereComponent;
//------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType)   
enum class Enum_Pickupable_Item : uint8
{
	EItem_Wick		UMETA(DisplayName="EItem_Wick"),
	EItem_Wax     UMETA(DisplayName="EItem_Wax"),
};
USTRUCT(BlueprintType)
struct FPickupableItem_Data
{
	GENERATED_BODY() 
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float Amount = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) Enum_Pickupable_Item Item_Type = Enum_Pickupable_Item::EItem_Wax;

};

//------------------------------------------------------------------------------------------------------------
UCLASS()
class LIVING_CANDLE_API APickupAble_Item : public AInteractable_Actor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APickupAble_Item();

	UFUNCTION(BlueprintCallable) virtual void Pickup(FPickupableItem_Data& data);

	UPROPERTY(BlueprintReadWrite, EditAnywhere) FPickupableItem_Data Item_Data;

	UPROPERTY(BlueprintReadWrite) UInteract_SphereComponent * Pickup_Sphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	



};
//------------------------------------------------------------------------------------------------------------
