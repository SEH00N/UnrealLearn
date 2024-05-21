// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_LessHP.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UBTDecorator_LessHP : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_LessHP();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
