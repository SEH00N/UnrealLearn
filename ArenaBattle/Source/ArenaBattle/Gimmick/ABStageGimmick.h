// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};

DECLARE_DELEGATE(FOnStageStateChangedDelegate);

USTRUCT(BlueprintType)
struct FStageStateChangedDelegateWrapper
{
	GENERATED_BODY()

	FStageStateChangedDelegateWrapper() {}
	FStageStateChangedDelegateWrapper(const FOnStageStateChangedDelegate& InDelegate) : StageDeletage(InDelegate) {}

	FOnStageStateChangedDelegate StageDeletage;
};

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	AABStageGimmick();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Stage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;

	UPROPERTY(VisibleAnywhere, Category = "Stage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnStageTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Gate", meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	UPROPERTY(VisibleAnywhere, Category = "Gate", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	UFUNCTION()
	void OnGateTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGates();
	void CloseAllGates();

protected:
	UPROPERTY(EditAnywhere, Category = "Stage", meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	UPROPERTY()
	TMap<EStageState, FStageStateChangedDelegateWrapper> StateChangedActions;

	void SetState(EStageState InNewState);
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

protected:
	UPROPERTY(EditAnywhere, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABCharacterNonPlayer> OpponentClass;
	
	UPROPERTY(EditAnywhere, Category = "Fight", meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	FTimerHandle OpponentTimerHandle;

	void OnOpponentSpawn();

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);

protected:
	UPROPERTY(EditAnywhere, Category = "Reward", meta = (AllowPrivateAccess = "True"))
	TSubclassOf<class AABItemBox> RewardBoxClass;

	UPROPERTY(EditAnywhere, Category = "Reward", meta = (AllowPrivateAccess = "True"))
	TArray<TWeakObjectPtr<class AABItemBox>> RewardBoxes;

	TMap<FName, FVector> RewardBoxLocations;

	void SpawnRewardBoxes();

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
