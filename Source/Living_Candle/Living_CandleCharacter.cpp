// Copyright Epic Games, Inc. All Rights Reserved.

#include "Living_CandleCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Water_Actor.h"
#include "Interact_CapsuleComponent.h"
#include "Interact_BoxComponent.h"
#include "Interact_SphereComponent.h"
//#include "AInteractable_Actor.h"
#include "Melee_Weapon.h"
#include "PickupAble_Item.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//------------------------------------------------------------------------------------------------------------
// ALiving_CandleCharacter
ALiving_CandleCharacter::ALiving_CandleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)



	//Create a Head_Wick_Collision and Begin Overlap Event
	Head_Wick_Collision = CreateDefaultSubobject<UInteract_SphereComponent>(TEXT("Head_Wick_Collision"));
	Head_Wick_Collision->SetupAttachment(GetMesh(),TEXT("Head_Wick_Collision_Socket"));
	Head_Wick_Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::Head_Wick_Collision_BeginOverlap);

	//
	Player_Widget_HUD = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player_Widget_HUD"));
	Player_Widget_HUD->SetupAttachment(RootComponent);
	
	//
	Wax_System = CreateDefaultSubobject<UWax_System_Comp>(TEXT("Wax_System"));

	//
	AbilitySystem_Comp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem_Comp"));

	//Attributes
	Base_AttributeSet = CreateDefaultSubobject<UBase_AttributeSet>(TEXT("Base_AttributeSet"));
	Attack_AttributeSet = CreateDefaultSubobject<UAttack_AttributeSet>(TEXT("Attack_AttributeSet"));

	//Attack_Comp
	//Attack_Comp = CreateDefaultSubobject<UAttack_Comp>(TEXT("Attack_Comp"));

	//Damage_System_Comp
	Damage_System_Comp = CreateDefaultSubobject<UDamage_System_Comp>(TEXT("Damage_System_Comp"));
}
//------------------------------------------------------------------------------------------------------------
void ALiving_CandleCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	// Get the appropriate Ability System Component. It could be on another Actor, so use GetAbilitySystemComponent and check that the result is valid.
	UAbilitySystemComponent* asc = GetAbilitySystemComponent();
	// Make sure the AbilitySystemComponent is valid. If failure is unacceptable, replace this if() conditional with a check() statement.
	if (IsValid(asc)) 
	{
		// Get the UMyAttributeSet from our Ability System Component. The Ability System Component will create and register one if needed.
		Base_AttributeSet = asc->GetSet<UBase_AttributeSet>();
		Attack_AttributeSet = asc->GetSet<UAttack_AttributeSet>();
		// We now have a pointer to the new UMyAttributeSet that we can use later. If it has an initialization function, this is a good place to call it.
	}
	
	//Attack_Comp
	Attack_Comp = GetComponentByClass<UAttack_Comp>();

	if (UPlayerHUD* playerhud = Cast<UPlayerHUD>(Player_Widget_HUD->GetUserWidgetObject()) )
	{
		PlayerHUD = playerhud;
		PlayerHUD->AddToPlayerScreen();
		Wax_System->Calculate_Max_HP();
		Wax_System->Owner_PlayerHUD = PlayerHUD;
		
		Wax_System->Update_Wick_Bar(); 
		Wax_System->Update_Wax_Bar(); 
		Wax_System->Check_Wick();
	}


}
//------------------------------------------------------------------------------------------------------------
// Input
void ALiving_CandleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALiving_CandleCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALiving_CandleCharacter::Look);

		// Use
		EnhancedInputComponent->BindAction(Action_Use, ETriggerEvent::Triggered, this, &ALiving_CandleCharacter::On_Action_Use);

		// Attack
		EnhancedInputComponent->BindAction(Action_Attack, ETriggerEvent::Triggered, this, &ALiving_CandleCharacter::On_Action_Attack);

		// AltAttack
		EnhancedInputComponent->BindAction(Action_AltAttack, ETriggerEvent::Triggered, this, &ALiving_CandleCharacter::On_Action_AltAttack);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//------------------------------------------------------------------------------------------------------------
//Event if something touched head wick
void ALiving_CandleCharacter::Head_Wick_Collision_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (Cast<AWater_Actor>(OtherActor)) 
	{
		Death(Enum_Death_Case::EDeath_Extinguished);

		
	}

}
//------------------------------------------------------------------------------------------------------------
//Handling incoming damage specific to this class (IDamage_Interface) 
void ALiving_CandleCharacter::Take_Damage(AActor *damage_causer, FDamage_Inf damage_info, bool &was_damaged)
{
	double result_damage = 0;

	//1. Calculate damage
	result_damage = damage_info.Pure_Damage + damage_info.Phys_Damage + damage_info.Fire_Damage;
	
	


	//2. Taking result damage (нужно адаптировать для атрибутов)
	Wax_System->Current_HP = Wax_System->Current_HP - result_damage;

	Wax_System->Update_Wick_Bar();
	Wax_System->Update_Wax_Bar();
	Wax_System->Check_Wick();

	if (Wax_System->Current_HP <= 0)
		Death(Enum_Death_Case::EDeath_WaxLoss);
}
//------------------------------------------------------------------------------------------------------------
//
void ALiving_CandleCharacter::Melee_Attack_TraceEnable()
{
	if(Attack_Comp->Current_Weapon != nullptr)
		Attack_Comp->Current_Weapon->Enable_Attack_Trace();

}
//------------------------------------------------------------------------------------------------------------
//
void ALiving_CandleCharacter::Melee_Attack_TraceDisable()
{
	if(Attack_Comp->Current_Weapon != nullptr)
		Attack_Comp->Current_Weapon->Disable_Attack_Trace();
}
//------------------------------------------------------------------------------------------------------------
//Call tracing funcion once from weapon if it exist
void ALiving_CandleCharacter::Weapon_Trace()
{
	if(Attack_Comp->Current_Weapon != nullptr)
		Attack_Comp->Current_Weapon->Attack_Trace();
}
//------------------------------------------------------------------------------------------------------------
void ALiving_CandleCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
//------------------------------------------------------------------------------------------------------------
void ALiving_CandleCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
//------------------------------------------------------------------------------------------------------------
//
void ALiving_CandleCharacter::On_Action_Use(const FInputActionValue& value)
{
	int i;
	double distance, min_distance = 0;
	AActor* item, * curr_item;
	FVector player_pos, item_pos;
	//FHitResult hit_result;

	GetCapsuleComponent()->GetOverlappingActors(Interactable_Actors, TSubclassOf<AInteractable_Actor>(AInteractable_Actor::StaticClass()) );

	//if(Pickup_Trace_Debug == false)//pickup using trace TEMP
	//{
	//	UKismetSystemLibrary::SphereTraceSingle(this, FVector(0.0, 0.0, 0.0), FVector(0.0, 0.0, 0.0), Pickup_Range, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::None, hit_result, true);
	//}
	//else
	//{
	//	UKismetSystemLibrary::SphereTraceMulti(this,FVector(0.0, 0.0, 0.0), FVector(0.0, 0.0, 0.0), Pickup_Range, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hit_result, true, FLinearColor::Red, FLinearColor::Green, Pickup_Debug_Duration);
	//}
	//item = hit_result.GetActor();

	Interactable_Actors.Remove(this);

	if(Attack_Comp->Current_Weapon != nullptr)
		Interactable_Actors.Remove(Attack_Comp->Current_Weapon);

	if (Interactable_Actors.Num() == 0)
		return;

	if (Interactable_Actors.Num() == 1)
	{
		item = Interactable_Actors[0];
		Interactable_Actors.RemoveAt(0);
	}
	else
	{
		player_pos = GetActorLocation();

		for (i = 0; i < Interactable_Actors.Num(); i++)
		{
			curr_item = Interactable_Actors[i];

			item_pos = curr_item->GetActorLocation();

			distance = FVector::Distance(player_pos, item_pos);

			if (i == 0 || distance < min_distance)//редко вылетает?? при подборе оружия, может это из-за одинаковой дистанции и нужно =<
			{
				min_distance = distance;
				item = curr_item;
			}

		}

		Interactable_Actors.Remove(item);
	}

	if (APickupAble_Item* pickupable_item = Cast<APickupAble_Item>(item))
		Pickup_Item(pickupable_item);

	if (AMelee_Weapon* weapon = Cast<AMelee_Weapon>(item))
		Pickup_Weapon(weapon);


}
//------------------------------------------------------------------------------------------------------------
//
void ALiving_CandleCharacter::On_Action_Attack(const FInputActionValue &value) 
{


}
//------------------------------------------------------------------------------------------------------------
//
void ALiving_CandleCharacter::On_Action_AltAttack(const FInputActionValue &value) 
{


}
//------------------------------------------------------------------------------------------------------------
//Pickup Weapon and modify it
void ALiving_CandleCharacter::Pickup_Weapon(AMelee_Weapon* weapon)
{
	if (weapon == nullptr)
		return;

	Drop_Current_Player_Weapon();

	Attack_Comp->Current_Weapon = weapon;

	Attack_Comp->Current_Weapon->Attach(GetMesh(), this);

	Have_Weapon = true;
	
	////Modify weapon (legacy?? now need use ability system)
	Attack_Comp->Current_Weapon->Weapon_CurrentDamage_Info.Phys_Damage = (Attack_Comp->Current_Weapon->Weapon_BaseDamage_Info.Phys_Damage + Attack_AttributeSet->GetAdded_Physical_Damage()) * Attack_AttributeSet->GetPhysical_Damage_Multiplier();//modify with Passive tree component
	Attack_Comp->Current_Weapon->Weapon_CurrentDamage_Info.Fire_Damage = Attack_Comp->Current_Weapon->Weapon_BaseDamage_Info.Fire_Damage * 1.0;
	Attack_Comp->Current_Weapon->Weapon_CurrentDamage_Info.Knockback = Attack_Comp->Current_Weapon->Weapon_BaseDamage_Info.Knockback * 1.0;
	Attack_Comp->Current_Weapon->Weapon_CurrentDamage_Info.Stun = Attack_Comp->Current_Weapon->Weapon_BaseDamage_Info.Stun * 1.0;

}
//------------------------------------------------------------------------------------------------------------
//Drop the current player's weapon, if any
void ALiving_CandleCharacter::Drop_Current_Player_Weapon()
{
	if (Attack_Comp->Current_Weapon != nullptr)
	{
		Attack_Comp->Current_Weapon->Detach();
		Attack_Comp->Current_Weapon = nullptr;
	}
		
}
//------------------------------------------------------------------------------------------------------------
//
void ALiving_CandleCharacter::Pickup_Item(APickupAble_Item* item)
{
	if (item == nullptr) 
	{

		return;
	}


	//item->Pickup();
	//Wick_Items++;

	Enum_Pickupable_Item pickupable_item_type;
	float amount = 0;

	item->Pickup(pickupable_item_type, amount);

	switch(pickupable_item_type)
	{
	case Enum_Pickupable_Item::EItem_Wick: 
	
		Wax_System->Update_Wick_Items(amount);
		break;
	case Enum_Pickupable_Item::EItem_Wax: 

		Wax_System->Heal_Or_UpdateWaxItems(amount);
		break;
	default: 
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("pickup type is undefined."));
		break;
	}

}
//------------------------------------------------------------------------------------------------------------
//What will happen before destroying
void ALiving_CandleCharacter::Death(Enum_Death_Case e_death_case)
{
	switch(e_death_case)
	{
	case Enum_Death_Case::EDeath_Extinguished: //death on full health(wax): from water drops, lack of oxygen,. the body will stay 
		Destroy();
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Extinguished. the body will stay"));//debug
		break;
	case Enum_Death_Case::EDeath_WaxLoss: //death from health(wax) loss. the body won't stay
		Drop_Current_Player_Weapon();
		Destroy();
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Wax Loss. the body won't stay"));//debug 
		break;
	default: 
		Destroy();
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Death case is undefined."));//debug
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
// Returns our Ability System Component.
UAbilitySystemComponent* ALiving_CandleCharacter::GetAbilitySystemComponent() const 
{
	return AbilitySystem_Comp; 
}
//------------------------------------------------------------------------------------------------------------
