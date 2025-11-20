// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TrapTestActor.h"

// Sets default values
ATrapTestActor::ATrapTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	TrapCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));

	Trap = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trap"));

	
}

// Called when the game starts or when spawned
void ATrapTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrapTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

