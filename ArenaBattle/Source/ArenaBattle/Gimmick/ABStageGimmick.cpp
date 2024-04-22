#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Item/ABItemBox.h"

AABStageGimmick::AABStageGimmick()
{
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshReference(TEXT("/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE"));
	if (StageMeshReference.Object)
	{
		Stage->SetStaticMesh(StageMeshReference.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	if (StageTrigger)
	{
		StageTrigger->SetupAttachment(Stage);
		StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
		StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
		StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerOverlap);
	}

	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate") ,TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshReference(TEXT("/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE"));
	if (GateMeshReference.Object)
	{
		for (FName GateSocket : GateSockets)
		{
			UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
			Gate->SetupAttachment(Stage, GateSocket);
			Gate->SetStaticMesh(GateMeshReference.Object);
			Gate->SetRelativeLocationAndRotation(FVector(0.0f, -80.5f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
			Gates.Add(GateSocket, Gate);

			FName TriggerName = *GateSocket.ToString().Append("Trigger");
			UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
			GateTrigger->SetupAttachment(Stage, GateSocket);
			GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
			GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
			GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
			GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerOverlap);
			GateTrigger->ComponentTags.Add(GateSocket);

			GateTriggers.Add(GateTrigger);
		}
	}

	CurrentState = EStageState::READY;
	StateChangedActions.Add(EStageState::READY, FStageStateChangedDelegateWrapper(FOnStageStateChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)));
	StateChangedActions.Add(EStageState::FIGHT, FStageStateChangedDelegateWrapper(FOnStageStateChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)));
	StateChangedActions.Add(EStageState::REWARD, FStageStateChangedDelegateWrapper(FOnStageStateChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward)));
	StateChangedActions.Add(EStageState::NEXT, FStageStateChangedDelegateWrapper(FOnStageStateChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext)));

	OpponentClass = AABCharacterNonPlayer::StaticClass();
	OpponentSpawnTime = 2.0f;

	RewardBoxClass = AABItemBox::StaticClass();
	for (FName GateSocket : GateSockets)
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);

	}
}

void AABStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(CurrentState);
}

void AABStageGimmick::OnStageTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void AABStageGimmick::OnGateTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(Stage->DoesSocketExist(SocketName));

	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllObjects,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		GetWorld()->SpawnActor<AABStageGimmick>(NewLocation, FRotator::ZeroRotator);
	}
}

void AABStageGimmick::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		Gate.Value->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;
	if (StateChangedActions.Contains(CurrentState))
		StateChangedActions[CurrentState].StageDeletage.ExecuteIfBound();
}

void AABStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	for (auto GateTrigger : GateTriggers)
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	OpenAllGates();
}

void AABStageGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	CloseAllGates();

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);
}

void AABStageGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	CloseAllGates();
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);

	OpenAllGates();
}

void AABStageGimmick::OnOpponentSpawn()
{
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);

	AABCharacterNonPlayer* ABOpponentCharacter = Cast<AABCharacterNonPlayer>(OpponentActor);
	if (ABOpponentCharacter)
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed);
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void AABStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value;
		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}

void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AABItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox)
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}
