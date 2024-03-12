// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ABCharacterPlayerClassReference(TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (ABCharacterPlayerClassReference.Class)
		DefaultPawnClass = ABCharacterPlayerClassReference.Class;

	PlayerControllerClass = AABPlayerController::StaticClass();
}
