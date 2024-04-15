// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestEnemy.h"
#include "Animation/EnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(0.84f, 0.84f, 0.84f));
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

void ATestEnemy::GetDamage(float damage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Find Anim Instance"));
		UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(AnimInstance);
		if (EnemyAnimInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("Find Enemy Anim Instance"));
			EnemyAnimInstance->PlayHitAnim();
		}
	}
}

