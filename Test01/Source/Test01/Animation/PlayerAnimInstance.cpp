// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerAnimInstance.h"
#include "Character/TestPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UPlayerAnimInstance::UPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAnimMontageRef(TEXT("/Game/Animation/AM_PlayerFire.AM_PlayerFire"));
	if (AttackAnimMontageRef.Object)
	{
		AttackAnimMontage = AttackAnimMontageRef.Object;
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<ATestPlayer>(ownerPawn);

	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		Speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = player->GetActorRightVector();
		Direction = FVector::DotProduct(rightVector, velocity);

		UCharacterMovementComponent* movement = player->GetCharacterMovement();
		if (movement)
			bIsInAir = movement->IsFalling();
	}
}

void UPlayerAnimInstance::PlayAttackAnim()
{
	Montage_Play(AttackAnimMontage);
}
