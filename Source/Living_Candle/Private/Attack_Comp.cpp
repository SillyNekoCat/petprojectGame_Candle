// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack_Comp.h"
#include "Interact_SphereComponent.h"
#include "GameFramework/Character.h"
//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UAttack_Comp::UAttack_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UAttack_Comp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UAttack_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
// 
void UAttack_Comp::OnMaxCombo_Reset_Index()
{
	if(Attack_Index >= SerialAttack_Montages.Num())
		Attack_Index = 0;
}
//------------------------------------------------------------------------------------------------------------
//
void UAttack_Comp::Attach_Weapon(AMelee_Weapon* melee_weapon)
{
	AActor* owner = GetOwner();

	ACharacter* character = Cast<ACharacter>(owner);

	//Validation
	if(!IsValid(character))
		return;

	USceneComponent *root_component = melee_weapon->GetRootComponent();

	if (UPrimitiveComponent* prim_component = Cast<UPrimitiveComponent>(root_component))
	{//Collision setup for attached weapon
		prim_component->SetSimulatePhysics(false);
		//prim_component->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		prim_component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		melee_weapon->Weapon_Pickup_Sphere->SetGenerateOverlapEvents(false);
	}

	// Attach to character
	FAttachmentTransformRules attachment_rules(EAttachmentRule::SnapToTarget, true);
	melee_weapon->AttachToComponent(character->GetMesh(), attachment_rules, FName(TEXT("Weapon_Socket")));

	melee_weapon->Owner_ASC = owner->GetComponentByClass<UAbilitySystemComponent>();
	//melee_weapon->Owner_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(owner);
	melee_weapon->Owner_AttackAttributeSet = Cast<UAttack_AttributeSet>(melee_weapon->Owner_ASC->GetAttributeSet(UAttack_AttributeSet::StaticClass()));
	
	melee_weapon->Weapon_Ignored_Actors.AddUnique(owner);

	melee_weapon->Call_OnInteract();

	Current_Weapon = melee_weapon;
}
//------------------------------------------------------------------------------------------------------------
//
void UAttack_Comp::Detach_CurrentWeapon()
{
	if (!IsValid(Current_Weapon))
		return;

	USceneComponent *root_component = Current_Weapon->GetRootComponent();

	Current_Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (UPrimitiveComponent* prim_component = Cast<UPrimitiveComponent>(root_component))
	{//Collision setup for detached weapon
		prim_component->SetSimulatePhysics(true);


		prim_component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Current_Weapon->Weapon_Pickup_Sphere->SetGenerateOverlapEvents(true);
	}

	
	Current_Weapon->Weapon_Ignored_Actors.Remove(GetOwner());
	//Owner_Of_Weapon = nullptr;
	Current_Weapon->Owner_ASC = nullptr;
	Current_Weapon->Owner_AttackAttributeSet = nullptr;

	Current_Weapon = nullptr;
}