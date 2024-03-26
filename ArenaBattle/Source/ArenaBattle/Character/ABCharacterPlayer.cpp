// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ABCharacterControlData.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionReference(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionReference.Object)
		JumpAction = JumpActionReference.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderMoveActionReference(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (ShoulderMoveActionReference.Object)
		ShoulderMoveAction = ShoulderMoveActionReference.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookActionReference(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (ShoulderLookActionReference.Object)
		ShoulderLookAction = ShoulderLookActionReference.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> QuaterMoveActionReference(TEXT("/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove"));
	if (QuaterMoveActionReference.Object)
		QuaterMoveAction = QuaterMoveActionReference.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeControlActionReference(TEXT("/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (ChangeControlActionReference.Object)
		ChangeControlAction = ChangeControlActionReference.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionReference(TEXT("/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionReference.Object)
		AttackAction = AttackActionReference.Object;

	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControlType(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUseControllerDesiredRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AABCharacterPlayer::SetCharacterControlType(ECharacterControlType NewCharacterControlType)
{
	UABCharacterControlData* NewCharacterControlData = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControlData);

	SetCharacterControlData(NewCharacterControlData);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControlData->InputMappingContext;
		if (NewMappingContext)
			Subsystem->AddMappingContext(NewMappingContext, 0);
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDireciton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightRirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDireciton, MovementVector.X);
	AddMovementInput(RightRirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	//float InputSizeSquared = MovementVector.SquaredLength();
	//float MovementVectorSize = 1.0f;
	//float MovementVectorSizeSquared = MovementVector.SquaredLength();

	//if (MovementVectorSizeSquared > 1.0f)
	//{
	//	MovementVector.Normalize();
	//	MovementVectorSizeSquared = 1.0f;
	//}
	//else
	//{
	//	MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	//}

	//FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0);
	//GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	//AddMovementInput(MoveDirection, MovementVectorSize);
	 
	MovementVector.Normalize();
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, 1.0f);
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	SetCharacterControlType((CurrentCharacterControlType == ECharacterControlType::Shoulder) ? ECharacterControlType::Quater : ECharacterControlType::Shoulder);
}

void AABCharacterPlayer::Attack()
{
	ProcessComboCommand();
}
