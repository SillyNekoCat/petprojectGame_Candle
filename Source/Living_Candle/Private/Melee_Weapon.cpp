// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee_Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Interact_SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
AMelee_Weapon::AMelee_Weapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//Revert_Weapon_Damage_Info();

	Weapon_Skeletal_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_Skeletal_Mesh"));

	Weapon_Pickup_Sphere = CreateDefaultSubobject<UInteract_SphereComponent>(TEXT("Weapon_Pickup_Sphere"));
	//Weapon_Pickup_Sphere->SetSphereRadius(10.f);
	Weapon_Pickup_Sphere->SetupAttachment(Weapon_Skeletal_Mesh);

	//
	Weapon_AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Weapon_AbilitySystemComp"));

	//
	Weapon_AttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("Weapon_AttributeSet"));
	//
	Base_AttributeSet = CreateDefaultSubobject<UBase_AttributeSet>(TEXT("Base_AttributeSet"));

	
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AMelee_Weapon::BeginPlay()
{
	Super::BeginPlay();

	//Trace_Delegate.BindUFunction(this, TEXT("Attack_Trace"));
	
	//AttackDuration_Delegate.BindUFunction(this, TEXT("AttackTrace_Duration"));

	Modify_Weapon_CurrentDamage_Info
	(Weapon_BaseDamage_Info.Phys_Damage, Weapon_BaseDamage_Info.Fire_Damage, Weapon_BaseDamage_Info.Knockback, Weapon_BaseDamage_Info.Stun);

	Knockback_Comp = GetComponentByClass<UKnockback_Comp>();

	//
	if (IsValid(Weapon_AbilitySystemComp)) 
	{
		// Get the UMyAttributeSet from our Ability System Component. The Ability System Component will create and register one if needed.
		Weapon_AttributeSet = Weapon_AbilitySystemComp->GetSet<UWeaponAttributeSet>();
		Base_AttributeSet = Weapon_AbilitySystemComp->GetSet<UBase_AttributeSet>();
		// We now have a pointer to the new UMyAttributeSet that we can use later. If it has an initialization function, this is a good place to call it.
	}
}
//------------------------------------------------------------------------------------------------------------
//Event OnComponentBeginOverlap
void AMelee_Weapon::Weapon_Hit_Capsule_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//FHitResult sweep_result = SweepResult; 

	//if(Owner_Of_Weapon != SweepResult.GetActor() )
		//Check_Hit(SweepResult);
		
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Weapon_Hit_Capsule_BeginOverlap"));
}
//------------------------------------------------------------------------------------------------------------
//timer function
//void AMelee_Weapon::AttackTrace_Duration()
//{
//
//	Disable_Attack_Trace();
//	Last_Touched_Actor = nullptr;//чтобы последующие атаки наносили урон одной и той же цели. 
//
//}
//------------------------------------------------------------------------------------------------------------
//
void AMelee_Weapon::Attach(USkeletalMeshComponent *arms_mesh, AActor* weapon_owner)
{
	USceneComponent *root_component = GetRootComponent();

	if (UPrimitiveComponent* prim_component = Cast<UPrimitiveComponent>(root_component))
	{//Collision setup for attached weapon
		prim_component->SetSimulatePhysics(false);
		//prim_component->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		prim_component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Weapon_Pickup_Sphere->SetGenerateOverlapEvents(false);
	}

	// аттачим оружие к нашему персонажу
	FAttachmentTransformRules attachment_rules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(arms_mesh, attachment_rules, FName(TEXT("Weapon_Socket")));

	Owner_Of_Weapon = weapon_owner;
	//Owner_ASC = Owner_Of_Weapon->GetComponentByClass<UAbilitySystemComponent>();
	Owner_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner_Of_Weapon);
	Weapon_Ignored_Actors.AddUnique(Owner_Of_Weapon);
	
	
	//Spec принадлежит владельцу оружи€
	//Weapon_Damage_Spec =  *Owner_ASC->MakeOutgoingSpec(GE_Damage_ToApply, 0, Owner_ASC->MakeEffectContext()).Data.Get(); 
	//»змен€ем базовые значени€ оружи€
}
//------------------------------------------------------------------------------------------------------------
//
void AMelee_Weapon::Detach()
{

	USceneComponent *root_component = GetRootComponent();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (UPrimitiveComponent* prim_component = Cast<UPrimitiveComponent>(root_component))
	{//Collision setup for detached weapon
		prim_component->SetSimulatePhysics(true);
		

		prim_component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Weapon_Pickup_Sphere->SetGenerateOverlapEvents(true);
	}


	Weapon_Ignored_Actors.Remove(Owner_Of_Weapon);
	Owner_Of_Weapon = nullptr;
	Owner_ASC = nullptr;

	//¬осстанавливаем базовые значени€ оружи€
	Revert_Weapon_Damage_Info();
}
//------------------------------------------------------------------------------------------------------------
//
void AMelee_Weapon::Enable_Attack_Trace()
{
	//GetWorldTimerManager().SetTimer(Trace_TimerHandle, Trace_Delegate, Attack_Trace_Interval, true, -1.0f);

	//if(is_alt_attack == false)
	//{
	//	GetWorldTimerManager().SetTimer(AttackDuration_TimerHandle, AttackDuration_Delegate, Attack_Trace_Duration, false, -1.0f);
	//}
	//else
	//{
	//	GetWorldTimerManager().SetTimer(AttackDuration_TimerHandle, AttackDuration_Delegate, AltAttack_Trace_Duration, false, -1.0f);
	//}




}
//------------------------------------------------------------------------------------------------------------
//
void AMelee_Weapon::Disable_Attack_Trace()
{
	//GetWorldTimerManager().ClearTimer(Trace_TimerHandle);

	Last_Touched_Actor = nullptr;

}
//------------------------------------------------------------------------------------------------------------
//validate result of trace, using him as parameter for delegate call On_SendTargets.
void AMelee_Weapon::Check_Hit(TArray <FHitResult> hits_results, TArray <UAbilitySystemComponent*> &ascs_apply_damage)
{

}
//------------------------------------------------------------------------------------------------------------
//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
TArray <UAbilitySystemComponent*> AMelee_Weapon::Attack_Trace()
{
	TArray <UAbilitySystemComponent*> ascs;
	return ascs;
}
//------------------------------------------------------------------------------------------------------------
//Revert Weapon_CurrentDamage_Info to base values Weapon_BaseDamage_Info
void AMelee_Weapon::Revert_Weapon_Damage_Info()
{
	//Weapon_CurrentDamage_Info.Phys_Damage = Weapon_BaseDamage_Info.Phys_Damage;
	//Weapon_CurrentDamage_Info.Fire_Damage = Weapon_BaseDamage_Info.Fire_Damage;
	//Weapon_CurrentDamage_Info.Knockback = Weapon_BaseDamage_Info.Knockback;
	//Weapon_CurrentDamage_Info.Stun = Weapon_BaseDamage_Info.Stun;

	
	
	//if (!Weapon_Damage_Spec.IsValid())
	//{
	//	return;
	//}
	//
	////Base phys damage
	//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Weapon_Damage_Spec, FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys")), Weapon_AttributeSet->Phys_Damage.GetBaseValue() ); 
	////Base fire damage
	//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Weapon_Damage_Spec, FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire")), Weapon_AttributeSet->Fire_Damage.GetBaseValue() ); 

	
}
//------------------------------------------------------------------------------------------------------------
//Change Weapon_BaseDamage_Info
void AMelee_Weapon::Set_Weapon_BaseDamage_Info(double phys, double fire, double knockback, double stun)
{


	if (phys == 0.0)
	{	}//nothing changes
	else
	{
		//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Weapon_Damage_Spec, FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys")), phys); 
		Weapon_BaseDamage_Info.Phys_Damage = phys;
	}

	if (fire == 0.0)
	{	}//nothing changes
	else
	{

		Weapon_BaseDamage_Info.Fire_Damage = fire;
	}

	if (knockback == 0.0)
	{	}//nothing changes
	else
	{
		Weapon_BaseDamage_Info.Knockback = knockback;
		if(Knockback_Comp != nullptr)
		{
			Knockback_Comp->Impulse = knockback;
			Knockback_Comp->Z_Impulse = knockback / 5;
		}
	}

	if (stun == 0.0)
	{	}//nothing changes
	else
	{
		Weapon_BaseDamage_Info.Stun = stun;
	}
	
	Weapon_CurrentDamage_Info = Weapon_BaseDamage_Info;
}
//------------------------------------------------------------------------------------------------------------
//Change Weapon_CurrentDamage_Info
void AMelee_Weapon::Modify_Weapon_CurrentDamage_Info(double phys, double fire, double knockback, double stun) 
{
	if (phys == 0.0)
	{	}//nothing changes
	else
	{
		Weapon_CurrentDamage_Info.Phys_Damage = phys;
	}

	if (fire == 0.0)
	{	}//nothing changes
	else
	{
		Weapon_CurrentDamage_Info.Fire_Damage = fire;
	}
	

	if (knockback == 0.0)
	{	}//nothing changes
	else
	{
		Weapon_CurrentDamage_Info.Knockback = knockback;
		if(Knockback_Comp != nullptr)
		{
			Knockback_Comp->Impulse = knockback;
			Knockback_Comp->Z_Impulse = knockback / 5;
		}
	}

	if (stun == 0.0)
	{	}//nothing changes
	else
	{
		Weapon_CurrentDamage_Info.Stun = stun;
	}


}
//------------------------------------------------------------------------------------------------------------
//
UAbilitySystemComponent* AMelee_Weapon::GetAbilitySystemComponent() const 
{
	return Weapon_AbilitySystemComp;
}
//------------------------------------------------------------------------------------------------------------

