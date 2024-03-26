// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"
#include "Player/ABPlayerController.h"

AABGameMode::AABGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ABCharacterPlayerClassReference(TEXT("/Game/ArenaBattle/Blueprint/BP_ABCharacterPlayer.BP_ABCharacterPlayer_C"));
	if (ABCharacterPlayerClassReference.Class)
		DefaultPawnClass = ABCharacterPlayerClassReference.Class;

	PlayerControllerClass = AABPlayerController::StaticClass();
}
