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
	Montage_Play(HitAnimMontage);

	int Number = FMath::RandRange(1, 2);
	FName Section = *FString::Printf(TEXT("%s%d"), *SectionNamePrefix, Number);
	Montage_JumpToSection(Section, HitAnimMontage);
}
