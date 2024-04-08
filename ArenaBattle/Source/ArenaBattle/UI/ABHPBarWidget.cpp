// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHPBarWidget::UABHPBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHP = -1.0f;
}

void UABHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBHPBar")));
	ensure(HPProgressBar);

	IABCharacterWidgetInterface* characterWidgetInterface = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (characterWidgetInterface)
		characterWidgetInterface->SetUpCharacterWidget(this);
}

void UABHPBarWidget::UpdateHPBar(float NewCurrentHP)
{
	ensure(MaxHP > 0);
	if (HPProgressBar)
	{
		HPProgressBar->SetPercent(NewCurrentHP / MaxHP);
	}
}
