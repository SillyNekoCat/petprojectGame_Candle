// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire_Stream.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
AFire_Stream::AFire_Stream()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//


	//Start_Point
	Start_Point = CreateDefaultSubobject<USceneComponent>(TEXT("Start_Point"));



	//End_Point
	End_Point = CreateDefaultSubobject<USceneComponent>(TEXT("End_Point"));
	End_Point->SetupAttachment(Start_Point);
	//End_Point->AttachToComponent(Start_Point, FAttachmentTransformRules::SnapToTargetNotIncludingScale);


	//Capsule
	Capsule = CreateDefaultSubobject<UInteract_CapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Start_Point);
	//Capsule->AttachToComponent(Start_Point, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::Capsule_BeginOverlap);

	//ASC
	Owner_ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

}
//------------------------------------------------------------------------------------------------------------
// ConstructionScript
void AFire_Stream::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	this->SetActorScale3D(FVector(1.0, 1.0, 1.0) );
	End_Point->SetRelativeLocation(FVector(0.0, 0.0, Length));
	Calculate_CapsuleTraceShapes();
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void AFire_Stream::BeginPlay()
{
	Super::BeginPlay();
	
	//Setup Visibility
	Capsule->SetHiddenInGame(Is_Capsule_HiddenInGame);

	//Get Owner
	Owner_A = GetParentActor();
	if (IsValid(Owner_A) )
	{
		if(UAbilitySystemComponent* o_asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner_A) ) 
			Owner_ASC = o_asc;

	}
	else
	{
		
	}
	

	if(Owner_A != nullptr)
	{

		if (Ignore_Owner == true)
		{
			Trace_Ignore_Actors.Add(Owner_A);
		}
		
	}

	//Bind timer functions
	Dealing_DamageOverTime_Delegate.BindUFunction(this, TEXT("Dealing_DamageOverTime"));

	//Setup FlameStream Damage
	FlameStream_CurrentDamage_Info = FlameStream_BaseDamage_Info;

	//Check overlap on spawn
	//Capsule->GetOverlappingActors(Overlapping_Actors);
	//if (!Dealing_DamageOverTime_TimerHandle.IsValid() && Overlapping_Actors.IsEmpty() != true)
	Set_Dealing_DamageOverTime_Timer();


}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void AFire_Stream::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
//------------------------------------------------------------------------------------------------------------
//Event called when something starts to overlap Capsule
void AFire_Stream::Capsule_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//Capsule->GetOverlappingActors(Overlapping_Actors);

	if(Owner_A == nullptr || OtherActor != Owner_A)
	{
		if (!Dealing_DamageOverTime_TimerHandle.IsValid() )
			Set_Dealing_DamageOverTime_Timer();
	}
	

}
//------------------------------------------------------------------------------------------------------------
//
void AFire_Stream::Set_Dealing_DamageOverTime_Timer()
{
	GetWorldTimerManager().SetTimer(Dealing_DamageOverTime_TimerHandle, Dealing_DamageOverTime_Delegate, Trace_Interval, true, -1.0f);

}
//------------------------------------------------------------------------------------------------------------
// timer function, trace
void AFire_Stream::Dealing_DamageOverTime()
{
	//TArray <UAbilitySystemComponent*> ascs_apply_damage;

	TArray <AActor*> damage_actors;
	TArray <FHitResult> hits_results;
	FHitResult hit_result;


	Start_Loc = Start_Point->GetComponentLocation();
	End_Loc = End_Point->GetComponentLocation();


	//Trace
	if(Flame_Trace_Debug == false)
	{
		UKismetSystemLibrary::SphereTraceMulti(this, Start_Loc, End_Loc, Radius, ETraceTypeQuery::TraceTypeQuery1, false, Trace_Ignore_Actors, EDrawDebugTrace::None, hits_results, Is_TraceIgnoreSelf);
		//UKismetSystemLibrary::SphereTraceMultiByProfile(this, Start_Loc, End_Loc, Radius, UCollisionProfile::Pawn_ProfileName, false, Trace_Ignore_Actors, EDrawDebugTrace::None, hits_results, Is_TraceIgnoreSelf);

	}
	else
	{
		UKismetSystemLibrary::SphereTraceMulti(this, Start_Loc, End_Loc, Radius, ETraceTypeQuery::TraceTypeQuery1, false, Trace_Ignore_Actors, EDrawDebugTrace::ForDuration, hits_results, Is_TraceIgnoreSelf, FLinearColor::Red, FLinearColor::Green, Trace_Interval);
		//UKismetSystemLibrary::SphereTraceMultiByProfile(this, Start_Loc, End_Loc, Radius, UCollisionProfile::Pawn_ProfileName, false, Trace_Ignore_Actors, EDrawDebugTrace::ForDuration, hits_results, Is_TraceIgnoreSelf, FLinearColor::Red, FLinearColor::Green, Trace_Interval);

	}

	//Check hits
	for (int i = 0; i < hits_results.Num() ; i++) 
	{

		if (!Cast<UInteract_SphereComponent>(hits_results[i].GetComponent()) && !Cast<UInteract_CapsuleComponent>(hits_results[i].GetComponent()) && !Cast<UInteract_BoxComponent>(hits_results[i].GetComponent()))
		{
			damage_actors.AddUnique(hits_results[i].GetActor());
		}

	}

	//Apply damage
	for (int i = 0; i < damage_actors.Num() ; i++) 
	{
		if (damage_actors[i] != nullptr)
		{
			if (UAbilitySystemComponent* asc_damage_actor = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(damage_actors[i]) )//Damage_Actors[i]->GetComponentByClass<UAbilitySystemComponent>()
			{
				//ascs_apply_damage.Add(asc_damage_actor);

				FGameplayEffectSpec GE_Spec_Damage = *Owner_ASC->MakeOutgoingSpec(GE_Damage, 0, Owner_ASC->MakeEffectContext()).Data.Get();
				GE_Spec_Damage.SetSetByCallerMagnitude( FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire")), Fire_Damage);
				Owner_ASC->ApplyGameplayEffectSpecToTarget(GE_Spec_Damage, asc_damage_actor);
			}

		}
	}

	//Clear timer if no overlapping actors, exept owner
	Capsule->GetOverlappingActors(Overlapping_Actors);
	//if (IsValid(Owner_A))
		//Overlapping_Actors.Remove(Owner_A); //dissapears on player after first trace

	if (Overlapping_Actors.IsEmpty())
	{
		Clear_Dealing_DamageOverTime_Timer();
	}

}
//------------------------------------------------------------------------------------------------------------
//
void AFire_Stream::Clear_Dealing_DamageOverTime_Timer()
{
	GetWorldTimerManager().ClearTimer(Dealing_DamageOverTime_TimerHandle);
}
//------------------------------------------------------------------------------------------------------------
//Revert FlameStream_CurrentDamage_Info to base values FlameStream_BaseDamage_Info
void AFire_Stream::Revert_FlameStreamDamage()
{
	FlameStream_CurrentDamage_Info.Phys_Damage = FlameStream_BaseDamage_Info.Phys_Damage;
	FlameStream_CurrentDamage_Info.Fire_Damage = FlameStream_BaseDamage_Info.Fire_Damage;
	FlameStream_CurrentDamage_Info.Knockback = FlameStream_BaseDamage_Info.Knockback;
	FlameStream_CurrentDamage_Info.Stun = FlameStream_BaseDamage_Info.Stun;
}
//------------------------------------------------------------------------------------------------------------
//Change FlameStream_CurrentDamage_Info
void AFire_Stream::Modify_FlameStreamDamage(double phys, double fire, double knockback, double stun)
{

	if (phys == 0.0)
	{	}//nothing changes
	else
	{
		FlameStream_CurrentDamage_Info.Phys_Damage = phys;
	}

	if (fire == 0.0)
	{	}//nothing changes
	else
	{
		FlameStream_CurrentDamage_Info.Fire_Damage = fire;
	}

	if (knockback == 0.0)
	{	}//nothing changes
	else
	{
		FlameStream_CurrentDamage_Info.Knockback = knockback;
	}

	if (stun == 0.0)
	{	}//nothing changes
	else
	{
		FlameStream_CurrentDamage_Info.Stun = stun;
	}
}
//------------------------------------------------------------------------------------------------------------
//Calculate similar shapes for capsule and trace path
void AFire_Stream::Calculate_CapsuleTraceShapes()
{
	Start_Loc = Start_Point->GetComponentLocation();
	End_Loc = End_Point->GetComponentLocation();

	double length_of_vectors_difference = (Start_Loc - End_Loc).Length();

	Capsule->SetCapsuleSize(Radius, (length_of_vectors_difference / 2.0) + Radius, true);
	//Capsule->AddLocalOffset(FVector(0.0, 0.0, length_of_vectors_difference / 2.0));
	Capsule->SetRelativeLocation(FVector(0.0, 0.0, length_of_vectors_difference / 2.0));
}
//------------------------------------------------------------------------------------------------------------
//
UAbilitySystemComponent* AFire_Stream::GetAbilitySystemComponent() const
{
	return Owner_ASC;
}
//------------------------------------------------------------------------------------------------------------
