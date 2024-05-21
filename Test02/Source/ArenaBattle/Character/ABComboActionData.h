// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UABComboActionData();

public:
	UPROPERTY(EditAnywhere, Category = ComboAction)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = ComboAction)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = ComboAction)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboAction)
	TArray<float> EffectiveFrameCount;
};
