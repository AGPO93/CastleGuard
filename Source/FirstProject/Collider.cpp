// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("OurMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	CameraInput = FVector2D(0.0f, 0.0f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArmComponent->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.0f, -15.0f);
	SpringArmComponent->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);
}

void ACollider::MoveForward(float Input)
{
	FVector Forward = GetActorForwardVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Forward * Input);
	}
}

void ACollider::MoveRight(float Input)
{
	FVector Right = GetActorRightVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Right * Input);
	}
}

void ACollider::PitchCamera(float Axis)
{
	CameraInput.Y = Axis;
}

void ACollider::YawCamera(float Axis)
{
	CameraInput.X = Axis;
}

UPawnMovementComponent * ACollider::GetMovementComponent() const
{
	return OurMovementComponent;
}

// hello friend, this is a perforce test