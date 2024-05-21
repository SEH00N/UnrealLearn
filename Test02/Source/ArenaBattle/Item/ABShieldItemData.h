// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABShieldItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABShieldItemData : public UABItemData
{
	GENERATED_BODY()
	
public:
	UABShieldItemData();

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<UStaticMesh> ShieldMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat ModifierStat;
};
