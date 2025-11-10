// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ActionAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UActionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	if (IsValid(character)) {
		MovementComponent = character->GetCharacterMovement();
	}
}

void UActionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(MovementComponent)) {
		Speed = MovementComponent->Velocity.Size();
	}

}
