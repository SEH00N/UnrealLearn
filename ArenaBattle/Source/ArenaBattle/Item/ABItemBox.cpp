// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"

// Sets default values
AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	if (Trigger)
	{
		RootComponent = Trigger;
		Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->SetupAttachment(Trigger);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshReference(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
		if (BoxMeshReference.Object)
		{
			Mesh->SetStaticMesh(BoxMeshReference.Object);
			Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
		}
	}

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	if (Effect)
	{
		Effect->SetupAttachment(Trigger);
		static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectReference(TEXT("/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
		if (EffectReference.Object)
		{
			Effect->SetTemplate(EffectReference.Object);
			Effect->bAutoActivate = false;
		}
	}
}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}


