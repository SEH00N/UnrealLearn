// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHPBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	UABHPBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHP(float NewMaxHP) { MaxHP = NewMaxHP; }
	void UpdateHPBar(float NewCurrentHP);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HPProgressBar;

	UPROPERTY()
	float MaxHP;
};
