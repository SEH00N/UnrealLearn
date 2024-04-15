// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CrosshairWidget.h"
#include "Components/Image.h"

UCrosshairWidget::UCrosshairWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrosshairImage = Cast<UImage>(GetWidgetFromName(TEXT("Crosshair")));
	ensure(CrosshairImage);
}

void UCrosshairWidget::UpdateCrosshair(bool bTarget)
{
	if (CrosshairImage)
	{
		if(bTarget)
			CrosshairImage->SetColorAndOpacity(FLinearColor::Red);
		else
			CrosshairImage->SetColorAndOpacity(FLinearColor::Green);
	}
}
