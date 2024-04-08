// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPZeroDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHPZeroDelegate OnHPZeroEvent;
	FOnHPChangedDelegate OnHPChangedEvent;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHP;

public:	
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurrentHP() { return CurrentHP; }

	void SetHP(float NewHP);
	float ApplyDamage(float InDamage);
};
