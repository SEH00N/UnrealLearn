// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/EnemyAnimInstance.h"



UEnemyAnimInstance::UEnemyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitAnimMontageRef(TEXT("/Game/Animation/AM_EnemyHit.AM_EnemyHit"));
	if (HitAnimMontageRef.Object)
	{
		HitAnimMontage = HitAnimMontageRef.Object;
	}
}

void UEnemyAnimInstance::PlayHitAnim()
{
	StopAllMontages(0.0f);

	float const Duration = Montage_Play(HitAnimMontage, 1.0f);
	int32 index = FMath::RandRange(1, 2);
	FString HitAnimation = FString::Printf(TEXT("%s%d"), TEXT("Hit"), index);
	if (Duration > 0.0f)
		Montage_JumpToSection(*HitAnimation, HitAnimMontage);
}
