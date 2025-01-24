// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Heat_Component.h"
#include "Interact_CapsuleComponent.h"
#include "Interact_BoxComponent.h"
#include "Interact_SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


//------------------------------------------------------------------------------------------------------------
// Sets default values
ASword::ASword()
{

	Heat_Component = CreateDefaultSubobject<UHeat_Component>(TEXT("Heat_Component"));
	//OnDamage_TakeWDelegate.AddUObject(Heat_Component, &UHeat_Component::HeatDamage_Take); 
	Knockback_Comp = CreateDefaultSubobject<UKnockback_Comp>(TEXT("Knockback_Comp"));


}

//------------------------------------------------------------------------------------------------------------
//
void ASword::BeginPlay()
{
	Super::BeginPlay();

	//Correct call from parent
	//AMelee_Weapon::DispatchBeginPlay();
	//AMelee_Weapon::BeginPlay();

	//Делегат уведомляющий об получении урона минимум для Heat_Component
	//Base_AttributeSet->On_Damage_Take.AddDynamic(Heat_Component, &UHeat_Component::HeatDamage_Take);

}
//------------------------------------------------------------------------------------------------------------
//validate result of trace, using him as parameter for delegate call On_SendTargets.
void ASword::Check_Hit(TArray <FHitResult> hits_results, TArray <UAbilitySystemComponent*> &ascs_apply_damage)
{
	FDamage_Inf current_damage = Weapon_CurrentDamage_Info;//Без этого будет неправильный урон
	
	//for (int i = 0; i < hits_results.Num() ; i++) 
	//{

	//	AActor* hit_actor = hits_results[i].GetActor();
	//	UPrimitiveComponent* hit_comp = hits_results[i].GetComponent();

	//	if (hit_actor != nullptr && !Cast<UInteract_SphereComponent>(hit_comp) && !Cast<UInteract_CapsuleComponent>(hit_comp) && !Cast<UInteract_BoxComponent>(hit_comp))
	//	{
	//		if (Last_Touched_Actor != hit_actor)
	//		{
	//			if (hit_actor->Implements<UDamage_Interface>())
	//			{//Наносим урон только тем, кого мы ещё не касались за время атаки.

	//				if (Heat_Component->Accumulated_Heat != 0.0)//если оружие нагрето, то будет дополнительный урон от огня.
	//					Heat_Component->Calculate_HeatContactDamage(hit_actor, current_damage.Fire_Damage);

	//				Cast<IDamage_Interface>(hit_actor)->Take_Damage(Owner_Of_Weapon, current_damage, Was_Weapon_Damage_Applyed);

	//			}

	//			if (current_damage.Damage_Reaction == Enum_Damage_Reaction::EKnockback)
	//			{
	//				Knockback_Comp->Knockback_Impulse(hit_actor, hit_comp, Owner_Of_Weapon);//
	//			}
	//		}

	//		Last_Touched_Actor = hit_actor;
	//		//если нужно много ударов за атаку по одной и той же цели(например прикладывание бензопилы к врагу), то Last_Touched_Actor не нужно обрабатывать

	//	}
	//}

	//Check hits
	for (int i = 0; i < hits_results.Num(); i++)
	{

		if (!Cast<UInteract_SphereComponent>(hits_results[i].GetComponent()) && !Cast<UInteract_CapsuleComponent>(hits_results[i].GetComponent()) && !Cast<UInteract_BoxComponent>(hits_results[i].GetComponent()))
		{
			Damage_Actors.AddUnique(hits_results[i].GetActor());
			Hit_Components.AddUnique(hits_results[i].GetComponent());
		}


	}

	////Apply damage
	for (int i = 0; i < Damage_Actors.Num() ; i++) 
	{
		if (Damage_Actors[i] != nullptr && !Last_Touched_Actors.Contains(Damage_Actors[i]) )//Один удар за атаку
		{
			
			if (UAbilitySystemComponent* asc_damage_actor = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Damage_Actors[i]) )//Damage_Actors[i]->GetComponentByClass<UAbilitySystemComponent>()
			{
				//ASCs_ApplyDamage.Add(asc_damage_actor);
				Heat_Component->Calculate_HeatContactDamage(Damage_Actors[i]);

				Targets_Manager->Send_Targets(TArray <UAbilitySystemComponent*>{asc_damage_actor});
				//Тут должен вызыватся делегат из таргет компонента


			}

			//if (Damage_Actors[i]->Implements<UDamage_Interface>() )//old
			//{
			//	
			//	if (Heat_Component->Accumulated_Heat != 0.0)//если оружие нагрето, то будет дополнительный урон от огня.
			//		current_damage.Fire_Damage = Heat_Component->Calculate_HeatContactDamage(Damage_Actors[i]);

			//	Cast<IDamage_Interface>(Damage_Actors[i])->Take_Damage(Owner_Of_Weapon, current_damage, Was_Weapon_Damage_Applyed);
			//}

		}
		
	}
	
	//Apply impulse
	if (current_damage.Knockback > 0.0 && Knockback_Comp != nullptr)
	{
		for (int i = 0; i < Hit_Components.Num(); i++)
		{
			if (Hit_Components[i] != nullptr && !Last_Touched_Comps.Contains(Hit_Components[i]) )
			{
				
				Knockback_Comp->Knockback_Impulse(Hit_Components[i]->GetOwner(), Hit_Components[i], Owner_Of_Weapon);

			}
			
		}
	}

	Last_Touched_Actors = Damage_Actors;
	Last_Touched_Comps = Hit_Components;

	ascs_apply_damage = ASCs_ApplyDamage;

	Damage_Actors.Empty();
	Hit_Components.Empty();
	ASCs_ApplyDamage.Empty();
	
}
//------------------------------------------------------------------------------------------------------------
//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
TArray <UAbilitySystemComponent*> ASword::Attack_Trace()
{
	//FHitResult hit_result;
	TArray <FHitResult> hits_results;
	TArray <UAbilitySystemComponent*> ascs;

	if(Weapon_Trace_Debug == false)
	{
		UKismetSystemLibrary::SphereTraceMulti(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")), Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), Trace_Radius, ETraceTypeQuery::TraceTypeQuery1, Is_Trace_Complex, Weapon_Ignored_Actors, EDrawDebugTrace::None, hits_results, true);
		//UKismetSystemLibrary::SphereTraceSingle(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")),  Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), Trace_Radius, ETraceTypeQuery::TraceTypeQuery1, Is_Trace_Complex, Weapon_Ignored_Actors, EDrawDebugTrace::None, hit_result, true);
		//UKismetSystemLibrary::BoxTraceSingle(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")),  Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), Shape_HalfSize, Shape_Rotator, ETraceTypeQuery::TraceTypeQuery1, false, Weapon_Ignored_Actors, EDrawDebugTrace::None, hit_result, true);
		//UKismetSystemLibrary::LineTraceSingle(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")), Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), ETraceTypeQuery::TraceTypeQuery1, false, Weapon_Ignored_Actors, EDrawDebugTrace::None, hit_result, true);
	}
	else
	{
		UKismetSystemLibrary::SphereTraceMulti(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")), Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), Trace_Radius, ETraceTypeQuery::TraceTypeQuery1, Is_Trace_Complex, Weapon_Ignored_Actors, EDrawDebugTrace::ForDuration, hits_results, true, FLinearColor::Red, FLinearColor::Green, Weapon_Trace_Debug_Duration);
		//UKismetSystemLibrary::SphereTraceSingle(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")), Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), Trace_Radius, ETraceTypeQuery::TraceTypeQuery1, Is_Trace_Complex, Weapon_Ignored_Actors, EDrawDebugTrace::ForDuration, hit_result, true, FLinearColor::Red, FLinearColor::Green, Weapon_Trace_Debug_Duration);
		//UKismetSystemLibrary::BoxTraceSingle(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")),  Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), Shape_HalfSize, Shape_Rotator, ETraceTypeQuery::TraceTypeQuery1, false, Weapon_Ignored_Actors, EDrawDebugTrace::ForDuration, hit_result, true, FLinearColor::Red, FLinearColor::Green, Weapon_Trace_Debug_Duration);
		//UKismetSystemLibrary::LineTraceSingle(this, Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket")), Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("EndTraceSocket")), ETraceTypeQuery::TraceTypeQuery1, false, Weapon_Ignored_Actors, EDrawDebugTrace::ForDuration, hit_result, true, FLinearColor::Red, FLinearColor::Green, Weapon_Trace_Debug_Duration);
	}


	Check_Hit(hits_results, ascs);
	return ascs;
}
//------------------------------------------------------------------------------------------------------------
//Handling incoming damage specific to this class (IDamage_Interface)
void ASword::Take_Damage(AActor* damage_causer, FDamage_Inf damage_info, bool& was_damaged)
{
	//damage_info.Fire_Damage = damage_info.Fire_Damage - Fire_Block;
	//if(damage_info.Fire_Damage < 0.0)
	//	damage_info.Fire_Damage = 0.0;


	//damage_info.Phys_Damage =
	//damage_info.Damage_Reaction =

	bool was_damage_taken = true;

	was_damaged = was_damage_taken;

	//OnDamage_TakeWDelegate.Broadcast(damage_info, was_damage_taken);

	//Heat_Component->HeatDamage_Take(damage_info, was_damage_taken); можно сделать проще без делегата

}
//------------------------------------------------------------------------------------------------------------
