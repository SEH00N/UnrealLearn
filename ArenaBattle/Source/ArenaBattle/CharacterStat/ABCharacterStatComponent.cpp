// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	MaxHP = 200.0f;
	CurrentHP = MaxHP;
}


void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHP(MaxHP);
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, MaxHP);
	OnHPChangedEvent.Broadcast(CurrentHP);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHP = CurrentHP;
	const float ActualDamage = FMath::Max<float>(InDamage, 0);

	SetHP(PrevHP - ActualDamage);

	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		OnHPZeroEvent.Broadcast();
	}

	return ActualDamage;
}

