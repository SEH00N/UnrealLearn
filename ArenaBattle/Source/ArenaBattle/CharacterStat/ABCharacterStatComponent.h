// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float /*Current HP*/)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UABCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurrentHP() { return CurrentHP; }

public:
	float ApplyDamage(float InDamage);
	void SetHP(float NewHP);

public:
	FOnHPZeroDelegate OnHPZeroEvent;
	FOnHPChangedDelegate OnHPChangedEvent;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHP;
};
