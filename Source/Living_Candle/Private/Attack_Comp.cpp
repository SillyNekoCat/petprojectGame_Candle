// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack_Comp.h"
#include "GameFramework/Character.h"
//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UAttack_Comp::UAttack_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//Bind timer function
	ActiveBlockRecovery_Delegate.BindUFunction(this, TEXT("ActiveBlockRecovery_TimerF"));
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
void UAttack_Comp::Set_ActiveBlockRecoveryTimer()
{
	GetOwner()->GetWorldTimerManager().SetTimer(ActiveBlockRecovery_TimerHandle, ActiveBlockRecovery_Delegate, ActiveBlockRecovery_TimerValue, true, -1.0f);
}
//------------------------------------------------------------------------------------------------------------
// 
void UAttack_Comp::Clear_ActiveBlockRecoveryTimer()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ActiveBlockRecovery_TimerHandle);
	//ActiveBlockRecovery_TimerHandle.Invalidate();
}
//------------------------------------------------------------------------------------------------------------
// Timer Function 
void UAttack_Comp::ActiveBlockRecovery_TimerF()
{
	if (Active_Block_Value >= MaxActive_Block_Value)
	{
		Active_Block_Value = MaxActive_Block_Value;
		Clear_ActiveBlockRecoveryTimer();
	}
	else
	{
		Active_Block_Value++;
	}

}
//------------------------------------------------------------------------------------------------------------
// return true if the block is completely broken or if causer is outside of the block area
bool UAttack_Comp::ActiveBlock_ReceiveHit(AActor* causer, int break_block_value)
{
	if (isActive_Block && Active_Block_Value > 0)
	{
		AActor* causer_0 = causer;
		if (Is_LookAt_Block(causer_0, GetOwner()))
		{
			Active_Block_Value = Active_Block_Value - break_block_value;
			Set_ActiveBlockRecoveryTimer();

			if (Active_Block_Value >= 0)
			{//block is not completely broken
				return false;
			}
			Active_Block_Value = 0;

		}

	}
	return true;
}
//------------------------------------------------------------------------------------------------------------
//
bool UAttack_Comp::Is_LookAt_Block(AActor* causer, AActor* blocking_actor)
{
	//UAttack_Comp* attack_comp = blocking_actor->GetComponentByClass<UAttack_Comp>();
	FVector start_loc = blocking_actor->GetActorLocation();
	FVector target_loc = causer->GetActorLocation();
	
	//////UKismetMathLibrary::FindLookAtRotation();
	FRotator find_look_at_rot = FRotationMatrix::MakeFromX(target_loc - start_loc).Rotator();
	if ( find_look_at_rot.Equals(Cast<APawn>(blocking_actor)->GetBaseAimRotation(), 45.0) )
	{//
		return true;
	}

	return false;
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
			melee_weapon->Weapon_PickupSphere->SetGenerateOverlapEvents(false);

	}

	// Attach to character
	FAttachmentTransformRules attachment_rules(EAttachmentRule::SnapToTarget, true);
	melee_weapon->AttachToComponent(character->GetMesh(), attachment_rules, FName(TEXT("Weapon_Socket")));

	melee_weapon->Owner_ASC = owner->GetComponentByClass<UAbilitySystemComponent>();
	//melee_weapon->Owner_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(owner);
	melee_weapon->Owner_AttackAttributeSet = Cast<UAttack_AttributeSet>(melee_weapon->Owner_ASC->GetAttributeSet(UAttack_AttributeSet::StaticClass()));

	melee_weapon->Owner_Of_Weapon = owner;

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
		Current_Weapon->Weapon_PickupSphere->SetGenerateOverlapEvents(true);
	}

	
	Current_Weapon->Weapon_Ignored_Actors.Remove(GetOwner());
	//Owner_Of_Weapon = nullptr;
	Current_Weapon->Owner_ASC = nullptr;
	Current_Weapon->Owner_AttackAttributeSet = nullptr;
	Current_Weapon->Owner_Of_Weapon = nullptr;
	Current_Weapon = nullptr;
}