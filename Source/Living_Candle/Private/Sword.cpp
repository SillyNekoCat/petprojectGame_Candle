// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Heat_Component.h"
#include "AbilitySystemBlueprintLibrary.h"


//------------------------------------------------------------------------------------------------------------
// Sets default values
ASword::ASword()
{
	


}

//------------------------------------------------------------------------------------------------------------
//
void ASword::BeginPlay()
{
	Super::BeginPlay();

	//Correct call from parent
	//AMelee_Weapon::DispatchBeginPlay();
	//AMelee_Weapon::BeginPlay();

	//ƒелегат уведомл€ющий об получении урона минимум дл€ Heat_Component
	//Base_AttributeSet->On_Damage_Take.AddDynamic(Heat_Component, &UHeat_Component::HeatDamage_Take);

}
//------------------------------------------------------------------------------------------------------------
//This function using for notify state. Call trace function that using weapon sockets to calculate end/start trace location, validate result, using him as parameter for delegate call On_SendTargets.(delegate is already called inside this or Check_Hit function)
void ASword::Attack_Trace()
{
	TArray <FHitResult> hits_results;
	
	UWorld* world = GetWorld();
	//FCollisionShape shape = FCollisionShape::MakeBox(CollisionShape_BoxHalfExtend);
	//FCollisionQueryParams collision_query_params = FCollisionQueryParams(FName(TEXT("BoxTraceMulti")), false);
	//collision_query_params.AddIgnoredActors(Weapon_Ignored_Actors);
	

	FVector start_socket_loc = Weapon_Skeletal_Mesh->GetSocketLocation(TEXT("StartTraceSocket"));

	if(Is_SweepBegin)//do once on start of attack
	{
		StartSweepLoc_Old = start_socket_loc;
		Old_ActorRotation = GetActorRotation();
	}

	//if (bool is_hit = world->SweepMultiByChannel(hits_results, StartSweepLoc_Old, start_socket_loc, Old_ActorRotation.Quaternion(), ECC_GameTraceChannel1, shape, collision_query_params))
	//{
	//	if(Weapon_Trace_Debug)
	//		DrawDebugBoxTraceMulti(world, StartSweepLoc_Old, start_socket_loc, CollisionShape_BoxHalfExtend, Old_ActorRotation, EDrawDebugTrace::ForDuration, is_hit, hits_results, FColor::Purple, FColor::Green, Weapon_Trace_Debug_Duration);
	//}
		
	if(Weapon_Trace_Debug == false)
	{
		UKismetSystemLibrary::BoxTraceMulti(world, StartSweepLoc_Old,  start_socket_loc, CollisionShape_BoxHalfExtend, Old_ActorRotation, ETraceTypeQuery::TraceTypeQuery1, false, Weapon_Ignored_Actors, EDrawDebugTrace::None, hits_results, true);
	}
	else
	{
		UKismetSystemLibrary::BoxTraceMulti(this, StartSweepLoc_Old,  start_socket_loc, CollisionShape_BoxHalfExtend, Old_ActorRotation, ETraceTypeQuery::TraceTypeQuery1, false, Weapon_Ignored_Actors, EDrawDebugTrace::ForDuration, hits_results, true, FLinearColor::Red, FLinearColor::Green, Weapon_Trace_Debug_Duration);
	}

	Old_ActorRotation = GetActorRotation();
	StartSweepLoc_Old = start_socket_loc;
	Is_SweepBegin = false;
	Check_Hit(hits_results);
}
//------------------------------------------------------------------------------------------------------------
