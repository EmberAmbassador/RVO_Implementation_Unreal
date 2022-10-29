// Copyright Epic Games, Inc. All Rights Reserved.

#include "RVOGameMode.h"
#include "RVOPlayerController.h"
#include "RVOCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARVOGameMode::ARVOGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARVOPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}