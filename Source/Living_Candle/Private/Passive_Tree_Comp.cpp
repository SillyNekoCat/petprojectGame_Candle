// Fill out your copyright notice in the Description page of Project Settings.


#include "Passive_Tree_Comp.h"

//------------------------------------------------------------------------------------------------------------
// Sets default values for this component's properties
UPassive_Tree_Comp::UPassive_Tree_Comp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	////// Создание дерева
	
	//Create start_node
	UPassive_Tree_Node* start_node = Create_Node(TEXT("start_node"), true, 0, 0, TArray<FNode_Bonus> {}); 
	
	//Create can_knockback_node
	FNode_Bonus node_bonus;
	node_bonus.Bonus_Type = EBonus_Type::EBonus_CanKnockback;
	node_bonus.Bonus_Value = 0;
	UPassive_Tree_Node* can_knockback_node = Create_Node(TEXT("can_knockback_node"), false, 1, 1, TArray<FNode_Bonus> {node_bonus});
	
	//Connect start_node -- can_knockback_node
	Create_Connection(start_node, can_knockback_node);


	
}
//------------------------------------------------------------------------------------------------------------
// Called when the game starts
void UPassive_Tree_Comp::BeginPlay()
{
	Super::BeginPlay();

	
	
}
//------------------------------------------------------------------------------------------------------------
// Called every frame
void UPassive_Tree_Comp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//------------------------------------------------------------------------------------------------------------
// Check can choosen node be allocated/upgraded
void UPassive_Tree_Comp::Check_Allocate(UPassive_Tree_Node* in_node)
{//Это прототип условия для аллокейта/прокачки.
	
	for (UPassive_Tree_Node* node : in_node->Connected_Nodes)//Перебираем все законекченные ноды
	{
		if (node->Node_Current_Level > 0 || node->Is_Start_Node)//Если нод вкачан или является стартовым
		{
			in_node->Node_Current_Level += 1;
			//Gain_Bonus(in_node)
		}
	}
}
//------------------------------------------------------------------------------------------------------------
// 
void UPassive_Tree_Comp::Gain_Node_Bonuses(UPassive_Tree_Node* in_node)
{
	//как будет работать получение бонусов из нода?

	for (FNode_Bonus bonus : in_node->Node_Bonuses)
	{

	}

}
//------------------------------------------------------------------------------------------------------------
//
UPassive_Tree_Node* UPassive_Tree_Comp::Create_Node(FName node_name,bool is_start_node, int node_max_level, int resource_requirement, TArray<FNode_Bonus> node_bonuses)
{//Создание нода и его добавление в виджет.

	UPassive_Tree_Node* new_node;
	new_node = CreateDefaultSubobject<UPassive_Tree_Node>(node_name);

	new_node->Is_Start_Node = is_start_node;
	new_node->Node_Max_Level = node_max_level;
	new_node->Resource_Requirement = resource_requirement;
	new_node->Node_Bonuses = node_bonuses;


	//Добавление в виджет
	//...

	//Возвращение ссылки на созданный объект
	return Existing_Nodes[Existing_Nodes.Add(new_node)]; 
	
	
}
//------------------------------------------------------------------------------------------------------------
// 
void UPassive_Tree_Comp::Create_Connection(UPassive_Tree_Node* node_one, UPassive_Tree_Node* node_two)
{//Создание связи между нодами и добавление этой связи в виджет.

	//КАК БУДЕТ СОЗДАВАТСЯ СВЯЗЬ НОДОВ? наверное в массив связей ноды будет заносится нода из массива дерева(Existing_Nodes)
	//Будет добавлять обоим операндам в Connected_Nodes ссылки друг на друга
	//Лучше использовать AddUnique?

	node_one->Connected_Nodes.AddUnique(node_two);
	node_two->Connected_Nodes.AddUnique(node_one);

	//добавление связи в виджет

}
//------------------------------------------------------------------------------------------------------------
