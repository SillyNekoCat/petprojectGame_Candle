// Fill out your copyright notice in the Description page of Project Settings.


#include "Heat_Actor.h"
#include "Heat_Component.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

//------------------------------------------------------------------------------------------------------------
AHeat_Actor::AHeat_Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = Can_Melting;
	
	Heat_Component = CreateDefaultSubobject<UHeat_Component>(TEXT("Heat_Component"));								
	//OnDamage_TakeDelegate.AddUObject(Heat_Component, &UHeat_Component::HeatDamage_Take);//temp old
	
	//
	AbilitySystem_Comp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem_Comp"));

	//
	Base_AttributeSet = CreateDefaultSubobject<UBase_AttributeSet>(TEXT("Base_AttributeSet"));

	//Targets_Manager
	Targets_Manager = CreateDefaultSubobject<UTargets_Manager>(TEXT("Targets_Manager"));
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AHeat_Actor::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the appropriate Ability System Component. It could be on another Actor, so use GetAbilitySystemComponent and check that the result is valid.
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	// Make sure the AbilitySystemComponent is valid. If failure is unacceptable, replace this if() conditional with a check() statement.
	if (IsValid(asc)) 
	{
		// Get the UMyAttributeSet from our Ability System Component. The Ability System Component will create and register one if needed.
		Base_AttributeSet = asc->GetSet<UBase_AttributeSet>();

		// We now have a pointer to the new UMyAttributeSet that we can use later. If it has an initialization function, this is a good place to call it.
	}

	////почему-то после изменени€ блюпринта делегат OnCheckHeat_Delegate не срабатывает в блюпринтах, но если перезагрузить уровень то всЄ работает.
	Check_HeatMelting();
	
	//ƒелегат уведомл€ющий об получении урона минимум дл€ Heat_Component
	//OnDamage_TakeDelegate.AddDynamic(Heat_Component, &UHeat_Component::HeatDamage_Take);
	//Base_AttributeSet->On_Damage_Take.AddUniqueDynamic(Heat_Component, &UHeat_Component::HeatDamage_Take);

	//
	Start_ActorScale = GetActorRelativeScale3D();//GetActorScale3D();

	//Get collision component only if it exists
	UPrimitiveComponent* collision_comp = GetComponentByClass<UShapeComponent>();

	if( Cast<UBoxComponent>(collision_comp))
	{
		Box_Comp = Cast<UBoxComponent>(collision_comp);
		Start_BoxExtent = Get_Shape_Value(Box_Comp);
	}

	if(collision_comp != nullptr)
		collision_comp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::Collision_ComponentBeginOverlap);

	//Bind timer functions
	CheckContactDamage_Delegate.BindUFunction(this, TEXT("HeatContact_Damage"));

}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void AHeat_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Check_HeatMelting();
	if (Is_Melting)
	{
		//if (Heat_Component->Heat_Status_Param < 1.0)
		//{
		//	Is_Melting = false;
		//}
		Check_HeatMelting();

		FVector target_melting_scale;
		Calculate_Melting_Scale(Start_ActorScale, GetActorRotation(), target_melting_scale);

		if (GetActorRelativeScale3D() != target_melting_scale)
		{
			
			this->SetActorScale3D(UKismetMathLibrary::VInterpTo(GetActorRelativeScale3D(), target_melting_scale, DeltaTime, Melting_InterpSpeed));
			Correct_Collision(Box_Comp, target_melting_scale);
		}

	}
	

}
//------------------------------------------------------------------------------------------------------------

void AHeat_Actor::Collision_ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	HeatContact_Damage();
	
	
	if (!CheckContactDamage_TimerHandle.IsValid() ) 
		Set_CheckContactDamage_Timer();

}
//------------------------------------------------------------------------------------------------------------
//
void AHeat_Actor::Set_CheckContactDamage_Timer()
{
	
	GetWorldTimerManager().SetTimer(CheckContactDamage_TimerHandle, CheckContactDamage_Delegate, ContactCheck_Interval, true, -1.0f);
	
}
//------------------------------------------------------------------------------------------------------------
//calculate contact fire damage
void AHeat_Actor::HeatContact_Damage()
{
	//bool was_damage_applyed;
	
	

	TArray<UPrimitiveComponent*> overlapping_components; 
	TArray <UAbilitySystemComponent*> ascs_apply_damage;
	
	GetOverlappingComponents(overlapping_components);
	

	for (int i = 0; i < overlapping_components.Num(); i++)
	{
		if (overlapping_components[i]->GetOwner() == this)
		{
			overlapping_components.RemoveSingle(overlapping_components[i]);
			continue;
		}

		if (!Cast<UInteract_SphereComponent>(overlapping_components[i]) && !Cast<UInteract_CapsuleComponent>(overlapping_components[i]) && !Cast<UInteract_BoxComponent>(overlapping_components[i]) )
		{
			AActor* comp_owner = overlapping_components[i]->GetOwner();
			
			
			Fire_Contact_Damage = Heat_Component->Calculate_HeatContactDamage(comp_owner);
			
			
			if (GEngine && do_once)//debug temp
			{
				do_once = false;
				GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, FString::Printf(TEXT("Fire_Contact_Damage = %f"), Fire_Contact_Damage));
			}
				
			
			//FDamage_Inf damage_info;
			//damage_info.Fire_Damage = fire_contact_damage;

			//Apply_Damage(comp_owner, damage_info, was_damage_applyed);

			//apply damage
			if (UAbilitySystemComponent* asc_damage_actor = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(comp_owner))
			{
				ascs_apply_damage.Add(asc_damage_actor);	
				
				Targets_Manager->Send_Targets(ascs_apply_damage);
				//Fire_Contact_Damage = 0.0;
			}

		}
	}	
	
	
	if(overlapping_components.IsEmpty())
		Clear_CheckContactDamage_Timer();


}
//------------------------------------------------------------------------------------------------------------
//
void AHeat_Actor::Clear_CheckContactDamage_Timer()
{
	GetWorldTimerManager().ClearTimer(CheckContactDamage_TimerHandle);


}
//------------------------------------------------------------------------------------------------------------
//
void AHeat_Actor::Apply_Damage(AActor* target, FDamage_Inf damage_info, bool& was_damage_applyed)
{

	if (target->Implements<UDamage_Interface>())
	{
		Cast<IDamage_Interface>(target)->Take_Damage(this, damage_info, was_damage_applyed);
	}
	//Targets_Manager
}
//------------------------------------------------------------------------------------------------------------
//Processing incoming damage(IDamage_Interface)
void AHeat_Actor::Take_Damage(AActor* damage_causer, FDamage_Inf damage_info, bool& was_damaged)
{
	//damage_info.Fire_Damage = damage_info.Fire_Damage - Fire_Block;
	//if(damage_info.Fire_Damage < 0.0)//validate
		//damage_info.Fire_Damage = 0.0;




	//damage_info.Phys_Damage =
	//damage_info.Damage_Reaction =

	bool was_damage_taken = true;

	was_damaged = was_damage_taken;

	if(damage_info.Fire_Damage > 0.0)
		//OnDamage_TakeDelegate.Broadcast(damage_info, was_damage_taken);//delete?
	

	
	//Check death
	if (Death_OnMaxHeat == true && Heat_Component->Heat_Status_Param >= 1.0)
	{
		Death();

	}


	Check_HeatMelting();

}
//------------------------------------------------------------------------------------------------------------
//What will happen before destroying
void AHeat_Actor::Death()
{
	
	Destroy();
}
//------------------------------------------------------------------------------------------------------------
//
void AHeat_Actor::Calculate_Melting_Scale(FVector old_scale, FRotator actor_rotation, FVector& result_melting_scale)
{
	FVector scale_result;
	double x_scale_result;
	double y_scale_result;
	double z_scale_result;

	//X melting scale
	if ( UKismetMathLibrary::NearlyEqual_FloatFloat(actor_rotation.Pitch, 90.0, 40.0)|| UKismetMathLibrary::NearlyEqual_FloatFloat(actor_rotation.Pitch, -90.0, 40.0) )
	{
		x_scale_result = old_scale.X / Melting_Scale_Divider;
		y_scale_result = old_scale.Y + (old_scale.X - x_scale_result) / 2.0;
		z_scale_result = old_scale.Z + (old_scale.X - x_scale_result) / 2.0;

		scale_result = FVector(x_scale_result,  y_scale_result, z_scale_result);
		
		result_melting_scale = scale_result;
		return;
	}

	//Y melting scale
	if ( UKismetMathLibrary::NearlyEqual_FloatFloat(actor_rotation.Roll, 90.0, 40.0)|| UKismetMathLibrary::NearlyEqual_FloatFloat(actor_rotation.Roll, -90.0, 40.0) )
	{
		y_scale_result = old_scale.Y / Melting_Scale_Divider;
		x_scale_result = old_scale.X + (old_scale.Y - y_scale_result) / 2.0;
		z_scale_result = old_scale.Z + (old_scale.Y - y_scale_result) / 2.0;

		scale_result = FVector(x_scale_result,  y_scale_result, z_scale_result);

		result_melting_scale = scale_result;
		return;
	}

	//Z melting scale
	z_scale_result = old_scale.Z / Melting_Scale_Divider;
	x_scale_result = old_scale.X + (old_scale.Z - z_scale_result) / 2.0;
	y_scale_result = old_scale.Y + (old_scale.Z - z_scale_result) / 2.0;

	scale_result = FVector(x_scale_result,  y_scale_result, z_scale_result);

	result_melting_scale = scale_result;

}
//------------------------------------------------------------------------------------------------------------
//
void AHeat_Actor::Check_HeatMelting()
{
	//Cant Melting
	if (Can_Melting == false || Heat_Component->Heat_Status_Param < 1.0)
	{
		Is_Melting = false;
	}

	//Can Melting  
	if (Can_Melting == true && Heat_Component->Heat_Status_Param >= 1.0)
	{
		Is_Melting = true;
		
	}

	OnCheckHeat_Delegate.Broadcast(Heat_Component->Heat_Status_Param);


}
//------------------------------------------------------------------------------------------------------------
//Get values of box
FVector AHeat_Actor::Get_Shape_Value(UBoxComponent* collision_comp)
{
	return collision_comp->GetUnscaledBoxExtent();
}
//------------------------------------------------------------------------------------------------------------
//
void AHeat_Actor::Correct_Collision(UBoxComponent* collision_comp, FVector scale)
{
	//FVector result;
	//Start_ShapeCollision_Value

	collision_comp->SetBoxExtent(Start_BoxExtent);//revert to start value

	//X melting scale
	if (scale.X < scale.Y && scale.X < scale.Z)
	{
		
		collision_comp->SetBoxExtent(Start_BoxExtent + FVector( 4.0, 0.0, 0.0) );

	}
	//Y melting scale
	if (scale.Y < scale.X && scale.Y < scale.Z)
	{
		
		collision_comp->SetBoxExtent(Start_BoxExtent + FVector( 0.0, 4.0, 0.0) );

	}
	//Z melting scale
	if (scale.Z < scale.Y && scale.Z < scale.X)
	{
		
		collision_comp->SetBoxExtent(Start_BoxExtent + FVector( 0.0, 0.0, 4.0) );

	}


}
//------------------------------------------------------------------------------------------------------------
//
UAbilitySystemComponent* AHeat_Actor::GetAbilitySystemComponent() const
{
	return AbilitySystem_Comp;
}
//------------------------------------------------------------------------------------------------------------
