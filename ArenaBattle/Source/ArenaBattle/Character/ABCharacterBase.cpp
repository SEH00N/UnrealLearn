// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABCharacterControlData.h"

AABCharacterBase::AABCharacterBase()
{
    bUseControllerRotationPitch = false;    // right vector rotate
    bUseControllerRotationYaw = false;      // up vector rotate
    bUseControllerRotationRoll = false;     // front vector rotate

    // capsule
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

    // mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshReference(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));
    if (CharacterMeshReference.Object)
        GetMesh()->SetSkeletalMesh(CharacterMeshReference.Object);

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassReference(TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C"));
    if (AnimInstanceClassReference.Class)
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassReference.Class);

    // Character Control Data Asset
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataReferece(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder"));
    if (ShoulderDataReferece.Object)
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataReferece.Object);

    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataReferece(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater"));
    if (QuaterDataReferece.Object)
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataReferece.Object);
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

}

