// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));	// 씬 컴포넌트 하나 만들기
	SetRootComponent(root);	//루트로 지정

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));	// TEXT는 컴포넌트 이름
	BodyMesh->SetupAttachment(root);	// root 아래에 붙이기
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
	BodyMesh->SetRelativeLocation(FVector(0, 0, 100));
	BodyMesh->SetRelativeRotation(FQuat());
}

void AFloatingActor::OnFloatingMeshUpdate(float DeltaTime)
{
	// 교수님 풀이 
	ElapsedTime += DeltaTime;
	UE_LOG(LogTemp, Log, TEXT("Elapsed Time : %.2f"), ElapsedTime);

	// Cos 함수 이용해서 위치 기준 업데이트 
	float CosValue = FMath::Cos(ElapsedTime);
	CosValue += 1;				// 2 -> 0 -> 2
	CosValue *= 0.5f;			// 1 -> 0 -> 1
	CosValue = 1 - CosValue;	// 0 -> 1 -> 0

	BodyMesh->SetRelativeLocation(FVector(0, 0, CosValue * MoveHeight));
	BodyMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 내 풀이
	//CountHeight += FMath::Cos(Speed);

	//if (CountHeight > MoveHeight)
	//{
	//	IsActorUp = !IsActorUp;
	//	CountHeight = 0.0f;
	//}
	//	
	////IsActorUp ? BodyMesh->AddRelativeLocation(DeltaTime * Speed * FVector::UpVector) : BodyMesh->AddRelativeLocation(-1 * DeltaTime * Speed * FVector::UpVector);
	//BodyMesh->AddRelativeRotation(FRotator(0, DeltaTime * SpinSpeed, 0));

	//IsActorUp ? BodyMesh->AddRelativeLocation(FMath::Cos(Speed) * FVector::UpVector) : BodyMesh->AddRelativeLocation(-1 * FMath::Cos(Speed) * FVector::UpVector);

	OnFloatingMeshUpdate(DeltaTime);
}

