// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEST01_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemyAnimInstance();

public:
	void PlayHitAnim();

public:
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	TObjectPtr<class UAnimMontage> HitAnimMontage;
};
