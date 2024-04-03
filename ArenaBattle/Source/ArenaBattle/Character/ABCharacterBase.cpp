// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABCharacterControlData.h"
#include "ABComboActionData.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include <kismet/GameplayStatics.h>
#include "CharacterStat/ABCharacterStatComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/ABUserWidget.h"
#include "UI/ABHPBarWidget.h"

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

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshReference(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/sk_CharM_Base.sk_CharM_Base"));
    if (CharacterMeshReference.Object)
        GetMesh()->SetSkeletalMesh(CharacterMeshReference.Object);

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassReference(TEXT("/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C"));
    if (AnimInstanceClassReference.Class)
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassReference.Class);

    // Character Control Data Asset
    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataReferece(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder"));
    if (ShoulderDataReferece.Object)
        CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataReferece.Object);

    static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataReferece(TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater"));
    if (QuaterDataReferece.Object)
        CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataReferece.Object);

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageReference(TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack"));
    if (ComboActionMontageReference.Object)
        ComboActionMontage = ComboActionMontageReference.Object;

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadActionMotage(TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead"));
    if (DeadActionMotage.Object)
        DeadMontage = DeadActionMotage.Object;

    static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataReference(TEXT("/Game/ArenaBattle/CharacterAction/ABComboActionData.ABComboActionData"));
    if (ComboActionDataReference.Object)
        ComboActionData = ComboActionDataReference.Object;

    Stat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("Stat"));
    HPBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
    HPBar->SetupAttachment(GetMesh());
    HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 225.0f));

    static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetReference(TEXT("/Game/ArenaBattle/UI/WBP_HPBar.WBP_HPBar_C"));
    if (HPBarWidgetReference.Class)
    {
        HPBar->SetWidgetClass(HPBarWidgetReference.Class);
        HPBar->SetWidgetSpace(EWidgetSpace::Screen);
        HPBar->SetDrawSize(FVector2D(150.0f, 15.0f));
        HPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void AABCharacterBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    Stat->OnHPZeroEvent.AddUObject(this, &AABCharacterBase::SetDead);
}

void AABCharacterBase::SetUpCharacterWidget(UABUserWidget* InUserWidget)
{
    UABHPBarWidget* HPBarWidget = Cast<UABHPBarWidget>(InUserWidget);
    if (HPBarWidget)
    {
        HPBarWidget->SetMaxHP(Stat->GetMaxHP());
        HPBarWidget->UpdateHPBar(Stat->GetCurrentHP());

        Stat->OnHPChangedEvent.AddUObject(HPBarWidget, &UABHPBarWidget::UpdateHPBar);
    }
}

void AABCharacterBase::AttackHitCheck()
{
    FHitResult OutHitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
    const float AttackRange = 40.0f;
    const float AttackRadius = 50.0f;
    const float AttackDamage = 30.0f;
    const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
    const FVector End = Start + GetActorForwardVector() * AttackRange;

    bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
    if (HitDetected)
    {
        FDamageEvent DamageEvent;
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
        //UGameplayStatics::ApplyDamage(OutHitResult.GetActor(), AttackDamage, GetController(), this, nullptr);
    }

#if ENABLE_DRAW_DEBUG
    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    float CapsuleHalfHeight = AttackRange * 0.5f;
    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    Stat->ApplyDamage(DamageAmount);

    return DamageAmount;
}

void AABCharacterBase::SetDead()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    PlayDeadAnimation();
    SetActorEnableCollision(false);
    HPBar->SetHiddenInGame(true);
}

void AABCharacterBase::PlayDeadAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->StopAllMontages(0.0f);
    AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

}

void AABCharacterBase::ProcessComboCommand()
{
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }

    if (ComboTimerHandle.IsValid())
        HasNextComboCommand = true;
    else
        HasNextComboCommand = false;
}

void AABCharacterBase::ComboActionBegin()
{
    CurrentCombo = 1;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    const float AttackSpeedRate = 1.0f;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
        AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AABCharacterBase::ComboActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMotage, bool IsPropertyEnded)
{
    ensure(CurrentCombo != 0);
    CurrentCombo = 0;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
    int32 ComboIndex = CurrentCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float AttackSpeedRate = 1.0f;
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

    if (ComboEffectiveTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AABCharacterBase::ComboCheck, ComboEffectiveTime, false);
    }
}

void AABCharacterBase::ComboCheck()
{
    ComboTimerHandle.Invalidate();
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        HasNextComboCommand = false;
    }
}
