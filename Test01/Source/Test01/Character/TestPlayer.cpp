// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystem.h"
#include <Blueprint/UserWidget.h>
#include "Animation/PlayerAnimInstance.h"
#include <Interface/DamageableInterface.h>

// Sets default values
ATestPlayer::ATestPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	// TPS카메라의 부모 클래스인 SpringArm 컴포넌트 붙이기
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	if (SpringArmComp)
	{
		SpringArmComp->SetupAttachment(RootComponent);
		SpringArmComp->SetRelativeLocation(FVector(0, 70, 90));
		SpringArmComp->TargetArmLength = 400;
		SpringArmComp->bUsePawnControlRotation = true;
	}

	// 카메라 컴포넌트 붙이기
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamComp"));
	if (CameraComp)
	{
		CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
		CameraComp->bUsePawnControlRotation = false;
	}

	// 스켈레탈메시
	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshRef(TEXT("/Game/Player/MW_Style2_Male.MW_Style2_Male"));
	if (PlayerMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(PlayerMeshRef.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 애님 인스턴스
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/ABP_Player.ABP_Player_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// 건 메시
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMeshRef(TEXT("/Game/Weapon/Mesh/SK_FPGun.SK_FPGun"));
	if (GunMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunMeshRef.Object);
		GunMeshComp->SetRelativeLocation(FVector(-17, 10, -3));
		GunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
	}

	// 무기 충돌 이펙트
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BulletHitEffectRef(TEXT("/Game/Paticles/P_BulletEffect.P_BulletEffect"));
	if (BulletHitEffectRef.Object)
	{
		BulletHitEffect = BulletHitEffectRef.Object;
	}

	// 크로스 헤어 UI
	static ConstructorHelpers::FClassFinder<UUserWidget> CroshairUIRef(TEXT("/Game/Widget/WBP_Crosshair.WBP_Crosshair_C"));
	if (CroshairUIRef.Class)
	{
		CrosshairUI = CroshairUIRef.Class;
	}

	// Input 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionForwardRef(TEXT("/Game/Input/IA_MoveForward.IA_MoveForward"));
	if (nullptr != InputActionForwardRef.Object)
	{
		MoveForwardAction = InputActionForwardRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRightRef(TEXT("/Game/Input/IA_MoveRight.IA_MoveRight"));
	if (nullptr != InputActionRightRef.Object)
	{
		MoveRightAction = InputActionRightRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionPitchRef(TEXT("/Game/Input/IA_TurnPitch.IA_TurnPitch"));
	if (nullptr != InputActionPitchRef.Object)
	{
		TurnPitchAction = InputActionPitchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionYawRef(TEXT("/Game/Input/IA_TurnYaw.IA_TurnYaw"));
	if (nullptr != InputActionYawRef.Object)
	{
		TurnYawAction = InputActionYawRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFireRef(TEXT("/Game/Input/IA_Fire.IA_Fire"));
	if (nullptr != InputActionFireRef.Object)
	{
		FireAction = InputActionFireRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Game/Input/IA_Run.IA_Run"));
	if (nullptr != InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}

	MoveSpeed = 200;
	RunSpeed = 600;
}

// Called when the game starts or when spawned
void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerUserWidget = CreateWidget(GetWorld(), CrosshairUI);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	if(PlayerUserWidget)
		PlayerUserWidget->AddToViewport();
}

// Called every frame
void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent) {

		//MoveForward
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATestPlayer::MoveForward);

		//MoveRight
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATestPlayer::MoveRight);

		//TurnPitch
		EnhancedInputComponent->BindAction(TurnPitchAction, ETriggerEvent::Triggered, this, &ATestPlayer::TurnPitch);

		//TurnYaw
		EnhancedInputComponent->BindAction(TurnYawAction, ETriggerEvent::Triggered, this, &ATestPlayer::TurnYaw);

		//Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATestPlayer::InputFire);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ATestPlayer::RunTrigger);
	}
}

void ATestPlayer::MoveForward(const FInputActionValue& Value)
{
	float Movement = Value.Get<float>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// add movement 
		AddMovementInput(ForwardDirection, Movement);
	}
}

void ATestPlayer::MoveRight(const FInputActionValue& Value)
{
	float Movement = Value.Get<float>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(RightDirection, Movement);
	}
}

void ATestPlayer::TurnPitch(const FInputActionValue& Value)
{
	float Turn = Value.Get<float>();
	if (Controller != nullptr)
	{
		AddControllerPitchInput(Turn);
	}
}

void ATestPlayer::TurnYaw(const FInputActionValue& Value)
{
	float Turn = Value.Get<float>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(Turn);
	}
}

void ATestPlayer::InputFire(const FInputActionValue& Value)
{
	UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
		AnimInstance->PlayAttackAnim();

	FVector StartPos = CameraComp->GetComponentLocation();
	FVector EndPos = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * 5000;

	FHitResult HitInfo;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bEnemyHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos, ECC_GameTraceChannel1, Params);
	if (bEnemyHit)
	{
		FTransform HitTransform;
		HitTransform.SetLocation(HitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitEffect, HitTransform);

		AActor* EnemyActor = HitInfo.GetActor();
		if (EnemyActor)
		{
			IDamageableInterface* Damageable = Cast<IDamageableInterface>(EnemyActor);
			if (Damageable)
				Damageable->GetDamage(100);
		}
	}

	bool bOtherHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos, ECC_Visibility, Params);
	if (bOtherHit)
	{
		FTransform HitTransform;
		HitTransform.SetLocation(HitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHitEffect, HitTransform);

		auto HitComp = HitInfo.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics())
		{
			FVector force = -HitInfo.ImpactNormal * HitComp->GetMass() * 500000;
			HitComp->AddForce(force);


		}
	}
}

void ATestPlayer::RunTrigger(const FInputActionValue& Value)
{
	bool bIsRun = Value.Get<bool>();
	if (bIsRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
}

