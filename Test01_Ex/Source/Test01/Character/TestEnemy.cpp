// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestEnemy.h"
#include "Animation/EnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TestCapsule"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMeshRef(TEXT("/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth"));
	if (EnemyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(EnemyMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/ABP_Enemy.ABP_Enemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestEnemy::OnDamageProcess()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(AnimInstance);
		EnemyAnimInstance->PlayHitAnim();
	}
}

