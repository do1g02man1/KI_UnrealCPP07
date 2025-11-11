// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh0"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0, 0, 250);
	SpringArm->bUsePawnControlRotation = true;		// 스프링암의 회전을 컨트롤러에 맞춤

	PlayerCamera = CreateDefaultSubobject <UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	
	bUseControllerRotationYaw = false;							// 컨트롤러의 Yaw회전을 사용 안함
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 무브먼트 방향으로 회전 조정 사용
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = GetMesh()->GetAnimInstance();	// ABP 객체 가져오기 
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeCount += DeltaTime;
	if (TimeCount > 3 && Stamina < 100.0f)
	{
		Stamina += 0.1f;
		UE_LOG(LogTemp, Log, TEXT("Stamina : %.1f"), Stamina);
	}
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* ehanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (ehanced)	// 입력 컴포넌트가 향상된 입력 컴포넌트일 때
	{
		ehanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveInput);
		ehanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started, 
			[this](const FInputActionValue& _) {
				SetSprintMode();
			});

		ehanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed, 
			[this](const FInputActionValue& _) {
				SetWalkMode();
			});
		ehanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AActionCharacter::OnRollInput);
		ehanced->BindAction(IA_Attack, ETriggerEvent::Started, this, &AActionCharacter::OnAttackInput);
	}
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();

	if (bIsSprint && (Stamina > 0.0f))
	{
		SetTimerDefault();
		Stamina -= RunStamina;
		UE_LOG(LogTemp, Log, TEXT("Stamina : %.1f"), Stamina);
	}
	else
	{
		SetWalkMode();
	}

	// Actor에 방향을 정해줘서 카메라가 돌아가도 움직이게 함
	// 내 풀이
	//AddMovementInput(GetActorRightVector(), inputDirection.X);
	//AddMovementInput(GetActorForwardVector(), inputDirection.Y);

	// 교수님 풀이
	FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);

	FQuat ControlYawRotation = FQuat(FRotator(0, GetControlRotation().Yaw, 0));
	moveDirection = ControlYawRotation.RotateVector(moveDirection);

	AddMovementInput(moveDirection);
	
}

void AActionCharacter::OnRollInput(const FInputActionValue& InValue)
{
	if (Stamina >= RollStamina)
	{
		if (AnimInstance.IsValid())
		{
			if (!AnimInstance->IsAnyMontagePlaying())
			{
				if (!GetLastMovementInputVector().IsNearlyZero())
				{
					SetActorRotation(GetLastMovementInputVector().Rotation());
				}
				SetTimerDefault();
				PlayAnimMontage(RollMontage);
				Stamina -= RollStamina;
				UE_LOG(LogTemp, Log, TEXT("Stamina : %.1f"), Stamina);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Stamina : %.1f"), Stamina);
			}
		}
	}
}

void AActionCharacter::OnAttackInput(const FInputActionValue& InValue)
{
	if (AnimInstance.IsValid())
	{
		if (!AnimInstance->IsAnyMontagePlaying())
		{
			PlayAnimMontage(AttackMontage);
		}
	}
}

void AActionCharacter::SetSprintMode()
{
	UE_LOG(LogTemp, Warning, TEXT("달리기 모드"));
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprint = true;
}

void AActionCharacter::SetWalkMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("걷기 모드"));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsSprint = false;
}

void AActionCharacter::SetTimerDefault()
{
	TimeCount = 0.0f;
}

