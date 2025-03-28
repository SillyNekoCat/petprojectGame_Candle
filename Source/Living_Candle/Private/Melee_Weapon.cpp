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
	//FHitResult sweep_result = SweepResult; 

	//if(Owner_Of_Weapon != SweepResult.GetActor() )
		//Check_Hit(SweepResult);
		
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Weapon_Hit_Capsule_BeginOverlap"));
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
//
//void AMelee_Weapon::Attach(USkeletalMeshComponent *arms_mesh, AActor* weapon_owner, UAttack_Comp* attack_comp)
//{
//
//
//	USceneComponent *root_component = GetRootComponent();
//
//	if (UPrimitiveComponent* prim_component = Cast<UPrimitiveComponent>(root_component))
//	{//Collision setup for attached weapon
//		prim_component->SetSimulatePhysics(false);
//		//prim_component->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
//		prim_component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//		Weapon_PickupSphere->SetGenerateOverlapEvents(false);
//	}
//
//	// Attach to character
//	FAttachmentTransformRules attachment_rules(EAttachmentRule::SnapToTarget, true);
//	AttachToComponent(arms_mesh, attachment_rules, FName(TEXT("Weapon_Socket")));
//
//	Owner_Of_Weapon = weapon_owner;
//	//Owner_ASC = Owner_Of_Weapon->GetComponentByClass<UAbilitySystemComponent>();
//	Owner_ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner_Of_Weapon);
//	Owner_AttackAttributeSet = Cast<UAttack_AttributeSet>(Owner_ASC->GetAttributeSet(UAttack_AttributeSet::StaticClass()));
//
//	Weapon_Ignored_Actors.AddUnique(Owner_Of_Weapon);
//
//	attack_comp->Current_Weapon = this;
//}
//------------------------------------------------------------------------------------------------------------
//
//void AMelee_Weapon::Detach(UAttack_Comp* attack_comp)
//{
//	USceneComponent *root_component = GetRootComponent();
//
//	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//
//	if (UPrimitiveComponent* prim_component = Cast<UPrimitiveComponent>(root_component))
//	{//Collision setup for detached weapon
//		prim_component->SetSimulatePhysics(true);
//		
//
//		prim_component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//		Weapon_PickupSphere->SetGenerateOverlapEvents(true);
//	}
//
//	attack_comp->Current_Weapon = nullptr;
//	Weapon_Ignored_Actors.Remove(Owner_Of_Weapon);
//	Owner_Of_Weapon = nullptr;
//	Owner_ASC = nullptr;
//	Owner_AttackAttributeSet = nullptr;
//}
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
		if (Damage_Actors[i] != nullptr && !Last_Touched_Actors.Contains(Damage_Actors[i]) )//ќдин удар за атаку
		{

			if (UAbilitySystemComponent* asc_damage_actor = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Damage_Actors[i]) )//Damage_Actors[i]->GetComponentByClass<UAbilitySystemComponent>()
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
						heatcomp_fire_damage = Heat_Component->Calculate_HeatContactDamage(Damage_Actors[i]);
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

	}

	////Apply impulse
	//if (current_damage.Knockback > 0.0 && Knockback_Comp != nullptr)
	//{
	//	for (int i = 0; i < Hit_Components.Num(); i++)
	//	{
	//		if (Hit_Components[i] != nullptr && !Last_Touched_Comps.Contains(Hit_Components[i]))
	//		{

	//			Knockback_Comp->Knockback_Impulse(Hit_Components[i]->GetOwner(), Hit_Components[i], Owner_Of_Weapon, 500.f);

	//		}

	//	}
	//}

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
//
UAbilitySystemComponent* AMelee_Weapon::GetAbilitySystemComponent() const 
{
	return Weapon_AbilitySystemComp;
}
//------------------------------------------------------------------------------------------------------------

