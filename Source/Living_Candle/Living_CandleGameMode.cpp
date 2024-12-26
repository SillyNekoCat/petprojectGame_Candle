// Copyright Epic Games, Inc. All Rights Reserved.

#include "Living_CandleGameMode.h"
#include "Living_CandleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALiving_CandleGameMode::ALiving_CandleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
