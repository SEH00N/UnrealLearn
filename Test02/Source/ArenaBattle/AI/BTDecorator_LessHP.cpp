// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_LessHP.h"
#include "ABAI.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ABCharacterAIInterface.h"

UBTDecorator_LessHP::UBTDecorator_LessHP()
{
	NodeName = TEXT("LessHP");
}

bool UBTDecorator_LessHP::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	bResult = AIPawn->GetAIHPRatio() <= AIPawn->GetAIRunHPRatio();

	return bResult;
}
