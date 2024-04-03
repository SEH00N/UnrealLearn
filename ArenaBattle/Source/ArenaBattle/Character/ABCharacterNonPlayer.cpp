// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshReference(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (CharacterMeshReference.Object)
		GetMesh()->SetSkeletalMesh(CharacterMeshReference.Object);
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle, 
		FTimerDelegate::CreateLambda([&]() {
			Destroy();
		}), 
		DeadEventDelayTime,
		false
	);
}
