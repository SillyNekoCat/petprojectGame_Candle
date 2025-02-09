// Fill out your copyright notice in the Description page of Project Settings.


#include "GEC_KnockbackExecution.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values
UGEC_KnockbackExecution::UGEC_KnockbackExecution()
{

}
//------------------------------------------------------------------------------------------------------------
// 
void UGEC_KnockbackExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* target_absc = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* target_actor = target_absc ? target_absc->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* source_absc = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* source_actor = source_absc ? source_absc->GetAvatarActor() : nullptr;

	//Get the owning GameplayEffect Spec so that you can use its variables and tags.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* source_tags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* target_tags = Spec.CapturedTargetTags.GetAggregatedTags();

	//Aggregator Evaluate Parameters used during the attribute capture.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = source_tags;
	EvaluationParameters.TargetTags = target_tags;	

	//Capturing  Magnitude
	float impulse = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Ability.Knockback.ImpulseMagnitude")), true, 0.0f);
	
	//Apply impulse
	Knockback_Impulse(target_actor, source_actor, impulse);

}
//------------------------------------------------------------------------------------------------------------
// Returns the impulse that is directed from the start(player) to the target
FVector UGEC_KnockbackExecution::Impulse_Calculate(FVector start, FVector target, float impulse, float z_impulse) const
{
	FVector multiplyed_vector = UKismetMathLibrary::Multiply_VectorVector(UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(start, target) ), FVector(impulse, impulse, impulse) );

	return UKismetMathLibrary::MakeVector(multiplyed_vector.X, multiplyed_vector.Y, z_impulse);


}
//------------------------------------------------------------------------------------------------------------
// Add impulse to Character_Movement_Component or root component that simulates physics
void UGEC_KnockbackExecution::Knockback_Impulse(AActor* actor, AActor* owner_of_weapon, float impulse) const
{
	AActor* input_actor = actor;
	AActor* input_owner_of_weapon = owner_of_weapon;
	float character_z_impulse = impulse / 2.f;
	float non_character_multiplyer = 150;
	float non_character_multiplyer_z = non_character_multiplyer / 100.f;


	FVector owner_forward = input_owner_of_weapon->GetActorForwardVector();

	if (input_actor != input_owner_of_weapon)
	{
		if (ACharacter* character = Cast<ACharacter>(input_actor))
		{

			//Cast<UCharacterMovementComponent>(character->GetMovementComponent())->AddImpulse(Impulse_Calculate(input_owner_of_weapon->GetActorLocation(), input_actor->GetActorLocation() ), true);
			character->GetCharacterMovement()->AddImpulse(Impulse_Calculate(input_owner_of_weapon->GetActorLocation(), input_actor->GetActorLocation(), impulse, character_z_impulse), true);
		}
		else
		{
			UPrimitiveComponent* comp = Cast<UPrimitiveComponent>(input_actor->GetRootComponent());
			if (comp->IsSimulatingPhysics())
			{
				//comp->AddImpulse(Impulse_Calculate(input_owner_of_weapon->GetActorLocation(), comp->GetComponentLocation(), impulse / 2,  impulse / 6), FName("None"), true);

				comp->AddImpulseAtLocation(UKismetMathLibrary::MakeVector(owner_forward.X * impulse * non_character_multiplyer, owner_forward.Y * impulse * non_character_multiplyer, owner_forward.Z * non_character_multiplyer_z), input_owner_of_weapon->GetActorLocation());
			}


			//if (input_comp->IsSimulatingPhysics())
			//{
			//	input_comp->AddImpulseAtLocation(UKismetMathLibrary::MakeVector(owner_forward.X * Impulse * 100, owner_forward.Y * Impulse * 100, owner_forward.Z * 100), input_owner_of_weapon->GetActorLocation());
			//}
		}
	}
}
//------------------------------------------------------------------------------------------------------------