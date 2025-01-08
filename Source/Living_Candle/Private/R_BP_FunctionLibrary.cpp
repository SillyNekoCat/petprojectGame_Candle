// Fill out your copyright notice in the Description page of Project Settings.


#include "R_BP_FunctionLibrary.h"
#include "EngineUtils.h"
//#include "Kismet/KismetSystemLibrary.h"
//------------------------------------------------------------------------------------------------------------
void UR_BP_FunctionLibrary::Visibility__GetRenderedActors(UObject* world_context_object, TArray<AActor*>& CurrentlyRenderedActors, float MinRecentTime)
{
	if(!world_context_object) return;

	UWorld* const world = GEngine->GetWorldFromContextObject(world_context_object, EGetWorldErrorMode::ReturnNull);
	if(!world) return;
	//~~~~~~~~~~~

	//Empty any previous entries
	CurrentlyRenderedActors.Empty();

	//Iterate Over Actors
	for ( TActorIterator<AActor> Itr(world); Itr; ++Itr )
	{
		if (world->GetTimeSeconds() - Itr->GetLastRenderTime() <= MinRecentTime)
		{
			CurrentlyRenderedActors.Add( * Itr);
		}
	} 
} 
//------------------------------------------------------------------------------------------------------------
void UR_BP_FunctionLibrary::Visibility__GetNotRenderedActors(UObject* world_context_object, TArray<AActor*>& CurrentlyNotRenderedActors, float MinRecentTime)
{
	if(!world_context_object) return;
	
	UWorld* const world = GEngine->GetWorldFromContextObject(world_context_object, EGetWorldErrorMode::ReturnNull);
	if(!world) return;
	//~~~~~~~~~~~

	//Empty any previous entries
	CurrentlyNotRenderedActors.Empty();

	//Iterate Over Actors
	for ( TActorIterator<AActor> Itr(world); Itr; ++Itr )
	{
		if (world->GetTimeSeconds() - Itr->GetLastRenderTime() > MinRecentTime)
		{
			CurrentlyNotRenderedActors.Add( * Itr);
		}
	}
} 
//------------------------------------------------------------------------------------------------------------
bool UR_BP_FunctionLibrary::Is_On_Screen(UObject* world_context_object, AActor* target, APlayerController* player_controller)
{
	if(!world_context_object) return false;

	UWorld* const world = GEngine->GetWorldFromContextObject(world_context_object, EGetWorldErrorMode::ReturnNull);
	if(!world) return false;


	int viewport_sizeX, viewport_sizeY;
	FVector2D screen_loc;

	FVector player_view_loc;
	FVector target_loc = target->GetActorLocation();
	FRotator player_view_rot;
	FHitResult hit_result;
	FCollisionResponseParams collison_response_p;
	FCollisionQueryParams  collision_query_p;  
	
	
	player_controller->GetViewportSize(viewport_sizeX, viewport_sizeY);
	
	player_controller->ProjectWorldLocationToScreen(target_loc, screen_loc, true);

	if(screen_loc.X > 0.0 && screen_loc.X < viewport_sizeX && screen_loc.Y > 0.0 && screen_loc.Y < viewport_sizeY)//is on screen
	{
		collision_query_p.AddIgnoredActor(target);

		player_controller->GetPlayerViewPoint(player_view_loc, player_view_rot);

		//trace to player viewpoint, to check can player see the target
		return !world->LineTraceSingleByChannel(hit_result, target_loc, player_view_loc, ECollisionChannel::ECC_Visibility, collision_query_p, collison_response_p);
	}
	else
	{
		return false;
	}

	
	//UKismetSystemLibrary::LineTraceSingle(GEngine->GetWorldFromContextObject(world_context_object, EGetWorldErrorMode::ReturnNull), target_loc, player_view_loc, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, hit_result, true);
	
}
//------------------------------------------------------------------------------------------------------------
