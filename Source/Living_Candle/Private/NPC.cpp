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

	//Widget_Component = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget_Component"));
	//Widget_Component->SetupAttachment(RootComponent);

	//
	AbilitySystem_Comp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem_Comp"));

	//
	Base_AttributeSet = CreateDefaultSubobject<UBase_AttributeSet>(TEXT("Base_AttributeSet"));
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Get the appropriate Ability System Component. It could be on another Actor, so use GetAbilitySystemComponent and check that the result is valid.
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	// Make sure the AbilitySystemComponent is valid. If failure is unacceptable, replace this if() conditional with a check() statement.
	if (IsValid(asc)) 
	{
		// Get the UMyAttributeSet from our Ability System Component. The Ability System Component will create and register one if needed.
		Base_AttributeSet = asc->GetSet<UBase_AttributeSet>();
		
		// We now have a pointer to the new UMyAttributeSet that we can use later. If it has an initialization function, this is a good place to call it.
	}

}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//------------------------------------------------------------------------------------------------------------
//
UAbilitySystemComponent* ANPC::GetAbilitySystemComponent() const
{
	return AbilitySystem_Comp;
}
//------------------------------------------------------------------------------------------------------------
