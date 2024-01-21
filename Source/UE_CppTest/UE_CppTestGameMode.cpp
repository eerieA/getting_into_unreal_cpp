// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_CppTestGameMode.h"
#include "UE_CppTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_CppTestGameMode::AUE_CppTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
