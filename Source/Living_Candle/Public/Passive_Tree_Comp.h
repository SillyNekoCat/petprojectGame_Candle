// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Passive_Tree_Comp.generated.h"
UENUM(BlueprintType)   
enum class EBonus_Type: uint8
{
	EBonus_Default			UMETA(DisplayName="EBonus_Default"),							//No changes, error
	EBonus_CanKnockback		UMETA(DisplayName="EBonus_CanKnockback"),					//
	EBonus_IncreasedKnockback		UMETA(DisplayName="EBonus_IncreasedKnockback"), //
	EBonus_CanStun				UMETA(DisplayName="EBonus_CanStun"),						//
	EBonus_IncreasedStun				UMETA(DisplayName="EBonus_IncreasedStun"),		//
	EBonus_CanThrowFlame				UMETA(DisplayName="EBonus_CanThrowFlame"),
};
//------------------------------------------------------------------------------------------------------------
//
USTRUCT(BlueprintType)
struct FNode_Bonus
{
	GENERATED_BODY()

public:
	//bonus type
	UPROPERTY(BlueprintReadWrite, EditAnywhere) EBonus_Type Bonus_Type = EBonus_Type::EBonus_Default;
	
	//bonus value
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Bonus_Value = 0;

};
//------------------------------------------------------------------------------------------------------------
//
UCLASS()
class UPassive_Tree_Node : public UObject
{
	GENERATED_BODY()

public:
	//как нод будет хранить получаемые бонусы?
	//как будет работать получение бонусов из нода?

	//Can be allocated without allocated connected nodes
	UPROPERTY(BlueprintReadWrite, EditAnywhere) bool Is_Start_Node = false;

	//How many times node can be upgraded, allocate also count as uprage
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Node_Max_Level = 1;

	//0 = node is not allocated
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Node_Current_Level = 0;

	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int Resource_Requirement = 1;
	
	//Бонусы от ноды
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<FNode_Bonus> Node_Bonuses;

	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<UPassive_Tree_Node*> Connected_Nodes;


};
//------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIVING_CANDLE_API UPassive_Tree_Comp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPassive_Tree_Comp();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<UPassive_Tree_Node*> Existing_Nodes;

	//////ПРОТОТИП
	

	//UFUNCTION(BlueprintCallable) void manage_node_connections? незнаю нужно ли это
	
	//Нужны ли все эти функции? Создание дерева через функции всё усложнит?
	//Редактирование дерева ведется через изменение информации о дереве, а функции будут использоватся для его отрисовки?

	UFUNCTION(BlueprintCallable) void Check_Allocate(UPassive_Tree_Node* in_node);
	UFUNCTION(BlueprintCallable) void Gain_Node_Bonuses(UPassive_Tree_Node* in_node);
	
	//создает ноду и задает ей данные, связи, добавляет её в массив Existing_Nodes
	UFUNCTION(BlueprintCallable) UPassive_Tree_Node* Create_Node(FName node_name, bool is_start_node, int node_max_level, int resource_requirement, TArray<FNode_Bonus> node_bonuses);
	//Создает связь между двумя нодами
	UFUNCTION(BlueprintCallable) void Create_Connection(UPassive_Tree_Node* node_one, UPassive_Tree_Node* node_two);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
//------------------------------------------------------------------------------------------------------------
