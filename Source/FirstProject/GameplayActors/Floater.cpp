// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));

	initialLocation = FVector(0.0f);
	placedLocation = FVector(0.0f);
	worldOrigin = FVector(0.0f, 0.0f, 0.0f);
	initialDirection = FVector(0.0f, 0.0f, 0.0f);

	bInitializeFloaterLocations = false;
	bShouldFloat = false;

	initialForce = FVector(200000.0f, 0.0f, 0.0f);
	initialTorque = FVector(200000.0f, 0.0f, 0.0f);

	runningTime = 0.0f;

	A = 0.0f;
	B = 0.0f;
	C = 0.0f;
	D = 0.0f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	placedLocation = GetActorLocation();

	if (bInitializeFloaterLocations)
	{
		SetActorLocation(initialLocation);
	}

	baseZLocation = placedLocation.Z;
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat)
	{
		FVector newLocation = GetActorLocation();

		newLocation.Z = baseZLocation + A * FMath::Sin(B * runningTime - C) + D;

		SetActorLocation(newLocation);

		runningTime += DeltaTime;
	}
}