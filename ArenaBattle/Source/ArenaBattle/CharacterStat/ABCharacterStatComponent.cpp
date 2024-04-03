// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHP = 200;

	SetHP(MaxHP);
}


void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHP(MaxHP);
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHP = CurrentHP;
	const float ActualDamage = FMath::Max<float>(InDamage, 0.0f);
	SetHP(PrevHP - ActualDamage);

	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		// Á×À½ Ã³¸®
		OnHPZeroEvent.Broadcast();
	}

	return ActualDamage;
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = FMath::Clamp<float>(NewHP, 0.0f, MaxHP);
	OnHPChangedEvent.Broadcast(CurrentHP);
}

