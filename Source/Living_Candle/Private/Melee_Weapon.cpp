// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee_Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Attack_Comp.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
AMelee_Weapon::AMelee_Weapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//Revert_Weapon_Damage_Info();
	
	//
	Weapon_Skeletal_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_Skeletal_Mesh"));

	//
	Weapon_PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon_PickupSphere"));
	Weapon_PickupSphere->ComponentTags.Add(TEXT("Ignore_Damage"));
	Weapon_PickupSphere->SetupAttachment(Weapon_Skeletal_Mesh);

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

	//
	On_Interact.AddDynamic(this, &ThisClass::Update_CustomEffectSpecs);

	//Trace_Delegate.BindUFunction(this, TEXT("Attack_Trace"));
	
	//AttackDuration_Delegate.BindUFunction(this, TEXT("AttackTrace_Duration"));
	
	//
	Heat_Component = GetComponentByClass<UHeat_Component>();

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

}
//------------------------------------------------------------------------------------------------------------
//
void AMelee_Weapon::Interact(AActor* actor)
{
	UAttack_Comp* attack_comp = actor->GetComponentByClass<UAttack_Comp>();

	if(!IsValid(attack_comp))
		return;

	//Drop current weapon
	attack_comp->Detach_CurrentWeapon();
	
	//Pickup this weapon
	attack_comp->Attach_Weapon(this);
}
//------------------------------------------------------------------------------------------------------------
//
void AMelee_Weapon::Update_CustomEffectSpecs_Implementation()
{

}
//------------------------------------------------------------------------------------------------------------
//validate result of trace
void AMelee_Weapon::Check_Hit(TArray <FHitResult> hits_results)
{
	

	//Check hits
	for (int i = 0; i < hits_results.Num(); i++)
	{

		if (!hits_results[i].GetComponent()->ComponentHasTag(TEXT("Ignore_Damage")))
		{
			Damage_Actors.AddUnique(hits_results[i].GetActor());
			Hit_Components.AddUnique(hits_results[i].GetComponent());
		}


	}

	////Apply damage
	for (int i = 0; i < Damage_Actors.Num() ; i++) 
	{
		if (Damage_Actors[i] != nullptr && !Last_Touched_Actors.Contains(Damage_Actors[i]) )//ќдин удар на цель за все трейсы
		{
			Apply_Weapon_Hit_Effects(Damage_Actors[i]);

		}

	}


	Last_Touched_Actors = Damage_Actors;

	Last_Touched_Comps = Hit_Components;

	Damage_Actors.Empty();
	Hit_Components.Empty();
	ASCs_ApplyDamage.Empty();
}
//------------------------------------------------------------------------------------------------------------
//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
void AMelee_Weapon::Attack_Trace()
{

}
//------------------------------------------------------------------------------------------------------------
//Applies all of the gameplay effects which are used by this weapon to hit (include damage)
void AMelee_Weapon::Apply_Weapon_Hit_Effects(AActor* target)
{
	if(UAttack_Comp* attack_comp = target->GetComponentByClass<UAttack_Comp>() )
	{
		if(!attack_comp->ActiveBlock_ReceiveHit(Owner_Of_Weapon, 1) )
			return;
	}


	if (UAbilitySystemComponent* asc_damage_actor = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(target) )//
	{
		//ASCs_ApplyDamage.Add(asc_damage_actor);


		////1. Damage
		//1.1 calculate damage
		float modified_phys_damage = Weapon_AttributeSet->GetPhys_Damage();
		if(IsValid(Owner_AttackAttributeSet))
			modified_phys_damage = (Weapon_AttributeSet->GetPhys_Damage() + Owner_AttackAttributeSet->GetMelee_Damage() + Owner_AttackAttributeSet->GetAdded_Physical_Damage() ) * Owner_AttackAttributeSet->GetPhysical_Damage_Multiplier();

		float heatcomp_fire_damage = 0.f;
		if(IsValid(Heat_Component))
		{
			if (Heat_Component->Can_Heat)
				heatcomp_fire_damage = Heat_Component->Calculate_HeatContactDamage(target);
		}

		//1.2 Make Effect Spec for Damage
		FGameplayEffectSpec GE_Spec_Damage = *Owner_ASC->MakeOutgoingSpec(GE_Damage_ToApply, 0, Owner_ASC->MakeEffectContext()).Data.Get();
		GE_Spec_Damage.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys")), modified_phys_damage);
		GE_Spec_Damage.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Damage_Resistance.Phys_Penetrate")), Weapon_AttributeSet->GetPhys_Penetrate());
		GE_Spec_Damage.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire")), Weapon_AttributeSet->GetFire_Damage() + heatcomp_fire_damage);
		GE_Spec_Damage.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("Damage_Resistance.Fire_Penetrate")), Weapon_AttributeSet->GetFire_Penetrate());

		//1.3 Apply Damage Effect
		Owner_ASC->ApplyGameplayEffectSpecToTarget(GE_Spec_Damage, asc_damage_actor);

		////2. Knockback
		float knockback_value = Owner_AttackAttributeSet->GetKnockback();
		if(knockback_value > 0.f)
		{
			//2.1 Make Effect Spec for Knockback
			FGameplayEffectSpec GE_Spec_Knockback = *Owner_ASC->MakeOutgoingSpec(GE_Knockback_ToApply, 0.f, Owner_ASC->MakeEffectContext()).Data.Get();
			GE_Spec_Knockback.SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Ability.Knockback.ImpulseMagnitude")), knockback_value);

			//2.2 Apply Knockback Effect
			Owner_ASC->ApplyGameplayEffectSpecToTarget(GE_Spec_Knockback, asc_damage_actor);
		}

		////3. Custom effect
		for(int e = 0; e < Custom_EffectSpecs.Num(); e++)
		{
			Owner_ASC->BP_ApplyGameplayEffectSpecToTarget(Custom_EffectSpecs[e], asc_damage_actor);
		}

	}
}
//------------------------------------------------------------------------------------------------------------
//
UAbilitySystemComponent* AMelee_Weapon::GetAbilitySystemComponent() const
{
	return Weapon_AbilitySystemComp;
}
//------------------------------------------------------------------------------------------------------------

