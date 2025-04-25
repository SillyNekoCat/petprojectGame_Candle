// Fill out your copyright notice in the Description page of Project Settings.


#include "GEC_DamageExecution.h"
#include "AbilitySystemComponent.h"
#include "Base_AttributeSet.h"

struct FDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(Incoming_Phys_Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Incoming_Fire_Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Incoming_Pure_Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Phys_Block);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Fire_Block);


	FDamageStatics()
	{
														
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBase_AttributeSet, Incoming_Phys_Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBase_AttributeSet, Incoming_Fire_Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBase_AttributeSet, Incoming_Pure_Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBase_AttributeSet, Phys_Block, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBase_AttributeSet, Fire_Block, Target, false);

	}

};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

//------------------------------------------------------------------------------------------------------------
// Sets default values
UGEC_DamageExecution::UGEC_DamageExecution()
{
	//RelevantAttributesToCapture.Add(DamageStatics().Phys_DamageDef);
	//RelevantAttributesToCapture.Add(DamageStatics().Fire_DamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().Incoming_Phys_DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().Incoming_Fire_DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().Incoming_Pure_DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().Phys_BlockDef);
	RelevantAttributesToCapture.Add(DamageStatics().Fire_BlockDef);
}
//------------------------------------------------------------------------------------------------------------
// 
void UGEC_DamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	
	//Capturing the weapon damage from Magnitude
	float base_phys_damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("DamageTypes.Phys")), true, 0.0f);
	float base_phys_pen = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage_Resistance.Phys_Penetrate")), true, 0.0f);

	float base_fire_damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("DamageTypes.Fire")), true, 0.0f);
	float base_fire_pen = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage_Resistance.Fire_Penetrate")), true, 0.0f); 

	float base_pure_damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("DamageTypes.Pure")), true, 0.0f);

	//Capturing Block attributes
	float phys_block = 0.f;
	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Phys_BlockDef, EvaluationParameters, phys_block) )
	{
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("GEC_DamageExecution FAIL ::: Capturing Phys_Block attributes"));//debug
	}
	
	float fire_block = 0.f;
	if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Fire_BlockDef, EvaluationParameters, fire_block))
	{
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("GEC_DamageExecution FAIL ::: Capturing Fire_Block attributes"));//debug
	}

	////Performing the actual damage calculation
	float phys_damage_done = base_phys_damage - (phys_block - base_phys_pen);
	if (phys_damage_done < 0.f)
	{
		phys_damage_done = 0.0f;

	}

	float fire_damage_done = base_fire_damage - (fire_block - base_fire_pen);
	if (fire_damage_done < 0.f)
	{
		fire_damage_done = 0.0f;

	}
	
	
	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ULyraHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	//Final execution output.
	//UBase_AttributeSet::GetIncoming_Phys_DamageAttribute()

	//OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();

	//Получается всегда будет 3 вызова
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().Incoming_Phys_DamageProperty, EGameplayModOp::Additive, phys_damage_done));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().Incoming_Fire_DamageProperty, EGameplayModOp::Additive, fire_damage_done));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().Incoming_Pure_DamageProperty, EGameplayModOp::Additive, base_pure_damage));

}
//------------------------------------------------------------------------------------------------------------
