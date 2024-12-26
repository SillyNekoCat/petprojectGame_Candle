// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee_Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Interact_SphereComponent.h"


//------------------------------------------------------------------------------------------------------------
// Sets default values
AMelee_Weapon::AMelee_Weapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Revert_Weapon_Damage_Info();

	Weapon_Skeletal_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_Skeletal_Mesh"));

	Weapon_Pickup_Sphere = CreateDefaultSubobject<UInteract_SphereComponent>(TEXT("Weapon_Pickup_Sphere"));
	//Weapon_Pickup_Sphere->SetSphereRadius(10.f);
	Weapon_Pickup_Sphere->SetupAttachment(Weapon_Skeletal_Mesh);

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
//Looping timer function using for trace
void AMelee_Weapon::Attack_Trace()
{


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
	Weapon_Ignored_Actors.AddUnique(Owner_Of_Weapon);

	//Изменяем базовые значения оружия

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

	//Восстанавливаем базовые значения оружия
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
//function using for calculate melee attack
void AMelee_Weapon::Check_Hit(TArray <FHitResult> hits_results)
{

}
//------------------------------------------------------------------------------------------------------------
//Revert Weapon_CurrentDamage_Info to base values Weapon_BaseDamage_Info
void AMelee_Weapon::Revert_Weapon_Damage_Info()
{
	Weapon_CurrentDamage_Info.Phys_Damage = Weapon_BaseDamage_Info.Phys_Damage;
	Weapon_CurrentDamage_Info.Fire_Damage = Weapon_BaseDamage_Info.Fire_Damage;
	Weapon_CurrentDamage_Info.Knockback = Weapon_BaseDamage_Info.Knockback;
	Weapon_CurrentDamage_Info.Stun = Weapon_BaseDamage_Info.Stun;
	
}
//------------------------------------------------------------------------------------------------------------
//Change Weapon_BaseDamage_Info
void AMelee_Weapon::Set_Weapon_BaseDamage_Info(double phys, double fire, double knockback, double stun)
{
	if (phys == 0.0)
	{	}//nothing changes
	else
	{
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
