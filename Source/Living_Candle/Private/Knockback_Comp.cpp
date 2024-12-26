// Fill out your copyright notice in the Description page of Project Settings.


#include "Knockback_Comp.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UKnockback_Comp::UKnockback_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//

}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UKnockback_Comp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UKnockback_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
// Returns the impulse that is directed from the start(player) to the target
FVector UKnockback_Comp::Impulse_Calculate(FVector start, FVector target)
{
	FVector multiplyed_vector = UKismetMathLibrary::Multiply_VectorVector(UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(start, target) ), FVector(Impulse, Impulse, Impulse) );
	
	return UKismetMathLibrary::MakeVector(multiplyed_vector.X, multiplyed_vector.Y, Z_Impulse);


}
//------------------------------------------------------------------------------------------------------------
// Add impulse to Character_Movement_Component or other component with simulate physics
void UKnockback_Comp::Knockback_Impulse(AActor* actor, UPrimitiveComponent* comp, AActor* owner_of_weapon)
{
	AActor* input_actor = actor;
	AActor* input_owner_of_weapon = owner_of_weapon;
	UPrimitiveComponent* input_comp = comp;

	FVector owner_forward = input_owner_of_weapon->GetActorForwardVector();

	if (input_actor != input_owner_of_weapon)
	{
		if (ACharacter* character = Cast<ACharacter>(input_actor))
		{

			  //Cast<UCharacterMovementComponent>(character->GetMovementComponent())->AddImpulse(Impulse_Calculate(input_owner_of_weapon->GetActorLocation(), input_actor->GetActorLocation() ), true);
			character->GetCharacterMovement()->AddImpulse(Impulse_Calculate(input_owner_of_weapon->GetActorLocation(), input_actor->GetActorLocation() ), Is_Velocity_Change);
		}
		else
		{
			if (input_comp->IsSimulatingPhysics())
			{
				
				//нужно сделать правильный импульс не зависящий от пивота цели
				input_comp->AddImpulseAtLocation(UKismetMathLibrary::MakeVector(owner_forward.X * Impulse * 100, owner_forward.Y * Impulse * 100, owner_forward.Z * 100), input_owner_of_weapon->GetActorLocation());
				
				//input_comp->AddImpulse(Impulse_Calculate(input_owner_of_weapon->GetActorLocation(), input_comp->GetComponentLocation() ), FName("None"), Is_Velocity_Change);
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------------
