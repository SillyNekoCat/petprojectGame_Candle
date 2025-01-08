// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/WidgetComponent.h"
//#include "R_BP_FunctionLibrary.h"
//------------------------------------------------------------------------------------------------------------
// Sets default values
ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Widget_Component = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget_Component"));
	Widget_Component->SetupAttachment(RootComponent);
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
//Handling incoming damage specific to this class (IDamage_Interface)
void ANPC::Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged)
{
	Current_HP = Current_HP - (damage_info.Phys_Damage + damage_info.Fire_Damage);


	//if (GEngine) //debug message on screen
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current_HP = %f"), Current_HP));

	if (Current_HP <= 0.0f)
		Destroy();

}
//------------------------------------------------------------------------------------------------------------
