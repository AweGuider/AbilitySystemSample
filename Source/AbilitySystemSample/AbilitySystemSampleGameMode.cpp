// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemSampleGameMode.h"
#include "AbilitySystemSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAbilitySystemSampleGameMode::AAbilitySystemSampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
