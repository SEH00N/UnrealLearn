// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindRunPos.h"
#include "ABAI.h"
#include "ABAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ABCharacterAIInterface.h"

UBTTask_FindRunPos::UBTTask_FindRunPos()
{
}

EBTNodeResult::Type UBTTask_FindRunPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float RunRadius = AIPawn->GetAIRunRadius();
	FNavLocation NextRunPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, RunRadius, NextRunPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_RUNPOS, NextRunPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
