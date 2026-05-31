// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeBase_Caine.h"

AGameModeBase_Caine::AGameModeBase_Caine()
{
	ConstructorHelpers::FClassFinder<APawn>MainCharacterCaine(TEXT("/Game/Caine/BP/BP_CharacterCaine"));
	DefaultPawnClass = MainCharacterCaine.Class;
}
