// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyActionAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void UMyActionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* onwerPawn = TryGetPawnOwner();
	if (onwerPawn)
	{
		OwnermovementComponent = onwerPawn->GetMovementComponent();
	}
}

void UMyActionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwnermovementComponent.IsValid())
	{
		Speed = OwnermovementComponent.Get()->Velocity.Size();
	}
}
