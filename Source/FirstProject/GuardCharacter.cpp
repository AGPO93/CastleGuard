// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"
#include "FirstSaveGame.h"
#include "CustomGameInstance.h"

// Sets default values
AGuardCharacter::AGuardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collisions in capsule
	GetCapsuleComponent()->SetCapsuleSize(52.3f, 111.12f);

	// Create camera boom (Pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	// Camera follows at this distance
	CameraBoom->TargetArmLength = 600.0f;
	// Rotate arm based on controller
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of spring arm,
	// let spring arm match controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	// Set turn rates for input
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	// Don't rotate when controller rotates, let that just affect the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement - character moves in the direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Rotation rate used by the above
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Player stats
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 150.f;
	MaxStamina = 150.f;
	Coins = 0;
	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;
	InterpSpeed = 15.f;

	//Initialise Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	bInterpToEnemy = false;
	bHasCombatTarget = false;
	bMovingForward = false;
	bMovingRight = false;
	bShiftKeyDown = false;
	bLMBDown = false;
	bESCDown = false;
}

// Called when the game starts or when spawned
void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	if (MainPlayerController)
	{
		FInputModeGameOnly InputMode;
		MainPlayerController->SetInputMode(InputMode);
	}

	UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetGameInstance());

	if (GameInstance && GameInstance->bSetPlayerData)
	{
		GameInstance->SetPlayerData();
	}
}

void AGuardCharacter::LoadPos()
{
	//UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetGameInstance());
	//if (GameInstance)
	//{
	//	UFirstSaveGame* LoadGameInstance = GameInstance->LoadData;

	//	Health = LoadGameInstance->PlayerStats.Health;
	//	MaxHealth = LoadGameInstance->PlayerStats.MaxHealth;
	//	Stamina = LoadGameInstance->PlayerStats.Stamina;
	//	MaxStamina = LoadGameInstance->PlayerStats.MaxStamina;
	//	Coins = LoadGameInstance->PlayerStats.Coins;

	//	if (WeaponStorage)
	//	{
	//		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);

	//		if (Weapons)
	//		{
	//			FString WeaponName = LoadGameInstance->PlayerStats.WeaponName;
	//			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, WeaponName + " LoadPos");

	//			if (Weapons->WeaponMap.Contains(WeaponName))
	//			{
	//				AWeapon* WeaponToSpawn = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
	//				WeaponToSpawn->Equip(this);
	//			}
	//		}
	//	}

	//	SetActorLocation(LoadGameInstance->PlayerStats.Location);
	//	SetActorRotation(LoadGameInstance->PlayerStats.Rotation);

	//	SetMovementStatus(EMovementStatus::EMS_Normal);

	//	GetMesh()->bPauseAnims = false;
	//}
}

// Called every frame
void AGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If dead, don't update other elements.
	if (MovementStatus == EMovementStatus::EMS_Dead) 
	{
		return;
	}

	float DeltaStamina = StaminaDrainRate * DeltaTime;
	DrainStamina(DeltaStamina);
	InterpToEnemy(DeltaTime);
	UpdateCombatTargetLocation();
}

void AGuardCharacter::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		FName CurrentLevelName(*CurrentLevel);

		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

void AGuardCharacter::SaveGame()
{
	UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetGameInstance());
	if (GameInstance)
	{

	}

	//UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	//SaveGameInstance->PlayerStats.Health = Health;
	//SaveGameInstance->PlayerStats.MaxHealth = MaxHealth;
	//SaveGameInstance->PlayerStats.Stamina = Stamina;
	//SaveGameInstance->PlayerStats.MaxStamina = MaxStamina;
	//SaveGameInstance->PlayerStats.Coins = Coins;
	//SaveGameInstance->PlayerStats.Location = GetActorLocation();
	//SaveGameInstance->PlayerStats.Rotation = GetActorRotation();

	//FString MapName = GetWorld()->GetMapName();
	//MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	//SaveGameInstance->PlayerStats.MapName = MapName;

	//if (EquippedWeapon)
	//{
	//	SaveGameInstance->PlayerStats.WeaponName = EquippedWeapon->Name;
	//}

	//UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SlotName, SaveGameInstance->SlotIndex);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("GAME SAVED"));

	//// Save to game instance
	//UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetGameInstance());
	//if (GameInstance) 
	//{
	//	GameInstance->LoadData = SaveGameInstance;
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GameInstance->LoadData->PlayerStats.WeaponName + " SaveGame");
	//}
}

void AGuardCharacter::LoadGame(bool SetPosition)
{
	//UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetGameInstance());
	//if (GameInstance)
	//{
	//	UFirstSaveGame* LoadGameInstance = GameInstance->LoadData;
	//	//UFirstSaveGame* LoadData = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	//	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SlotName, LoadGameInstance->SlotIndex));

	//	FString MapToLoad = LoadGameInstance->PlayerStats.MapName;
	//	FString CurrentMap = GetWorld()->GetMapName();
	//	CurrentMap.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	//	if (MapToLoad != CurrentMap && MapToLoad != TEXT(""))
	//	{
	//		GameInstance->bShouldLoad = true;
	//		FName MapName(LoadGameInstance->PlayerStats.MapName);
	//		SwitchLevel(MapName);
	//	}

	//	Health = LoadGameInstance->PlayerStats.Health;
	//	MaxHealth = LoadGameInstance->PlayerStats.MaxHealth;
	//	Stamina = LoadGameInstance->PlayerStats.Stamina;
	//	MaxStamina = LoadGameInstance->PlayerStats.MaxStamina;
	//	Coins = LoadGameInstance->PlayerStats.Coins;

	//	if (WeaponStorage)
	//	{
	//		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);

	//		if (Weapons)
	//		{
	//			FString WeaponName = LoadGameInstance->PlayerStats.WeaponName;
	//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, WeaponName + " LoadGame");

	//			if (Weapons->WeaponMap.Contains(WeaponName))
	//			{
	//				AWeapon* WeaponToSpawn = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
	//				WeaponToSpawn->Equip(this);
	//			}
	//		}
	//	}

	//	if (SetPosition)
	//	{
	//		SetActorLocation(LoadGameInstance->PlayerStats.Location);
	//		SetActorRotation(LoadGameInstance->PlayerStats.Rotation);
	//	}

	//	SetMovementStatus(EMovementStatus::EMS_Normal);

	//	GetMesh()->bPauseAnims = false;
	//	GetMesh()->bNoSkeletonUpdate = false;
	//}
}

void AGuardCharacter::LoadGameNoSwitch()
{
	/*UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SlotName, LoadGameInstance->SlotIndex));

	Health = LoadGameInstance->PlayerStats.Health;
	MaxHealth = LoadGameInstance->PlayerStats.MaxHealth;
	Stamina = LoadGameInstance->PlayerStats.Stamina;
	MaxStamina = LoadGameInstance->PlayerStats.MaxStamina;
	Coins = LoadGameInstance->PlayerStats.Coins;

	if (WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);

		if (Weapons)
		{
			FString WeaponName = LoadGameInstance->PlayerStats.WeaponName;

			if (Weapons->WeaponMap.Contains(WeaponName))
			{
				AWeapon* WeaponToSpawn = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
				WeaponToSpawn->Equip(this);
			}
		}
	}

	SetMovementStatus(EMovementStatus::EMS_Normal);

	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;*/
}

// Called to bind functionality to input
void AGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGuardCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AGuardCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AGuardCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AGuardCharacter::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AGuardCharacter::ESCUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AGuardCharacter::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AGuardCharacter::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGuardCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGuardCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AGuardCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AGuardCharacter::LookUp);

	PlayerInputComponent->BindAxis("TurnRate", this, &AGuardCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGuardCharacter::LookUpAtRate);
}

void AGuardCharacter::MoveForward(float Value)
{
	bMovingForward = false;

	if (CanMove(Value))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);

		bMovingForward = true;
	}
}
 
void AGuardCharacter::MoveRight(float Value)
{
	bMovingRight = false;

	if (CanMove(Value))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);

		bMovingRight = true;
	}
}

// Yaw Rotation
void AGuardCharacter::Turn(float Value)
{
	if (CanMove(Value))
	{
		AddControllerYawInput(Value);
	}
}

/** Called via input to turn at given rate
* @param Rate This is a normalized rate, i.e. 1.0 means 100% of the desired turn rate
*/
void AGuardCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

/** Called via input to look up/down at given rate
* @param Rate This is a normalized rate, i.e. 1.0 means 100% of the desired look up/down rate
*/
void AGuardCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Equipping weapon and attacking.
void AGuardCharacter::LMBDown()
{
	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_Dead)
	{
		return;
	}

	if (MainPlayerController)
	{
		if (MainPlayerController->bPauseMenuVisible) return;
	}

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
		}
	}
	else if (EquippedWeapon)
	{
		Attack();
	}
}

void AGuardCharacter::LMBUp()
{
	bLMBDown = false;
}

void AGuardCharacter::ESCDown()
{
	bESCDown = true;

	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

void AGuardCharacter::ESCUp()
{
	bESCDown = false;
}

void AGuardCharacter::Attack()
{
	if (!bAttacking && MovementStatus != EMovementStatus::EMS_Dead)
	{
		bAttacking = true;
		SetInterpToEnemy(true);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			default:
				break;
			}
		}
	}
}

// Reset bools, attack again if necessary.
void AGuardCharacter::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

void AGuardCharacter::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

// Stop animations after death.
void AGuardCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AGuardCharacter::DrainStamina(float DeltaStamina)
{
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown && (bMovingForward || bMovingRight))
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}

			Stamina -= DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}

			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;

	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown && (bMovingForward || bMovingRight))
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				SetMovementStatus(EMovementStatus::EMS_Normal);
				Stamina = 0;
			}
			else
			{

				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}

			Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;

	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.0f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}

		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}

		Stamina += DeltaStamina;
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	default:
		;
	}
}

bool AGuardCharacter::CanMove(float AxisValue)
{
	if (MainPlayerController)
	{
		return AxisValue != 0.0f && !bAttacking && 
			MovementStatus != EMovementStatus::EMS_Dead && 
			!MainPlayerController->bPauseMenuVisible;
	}
	else
	{
		return false;
	}
}

// Update location of combat target for enemy healthbar.
void AGuardCharacter::UpdateCombatTargetLocation()
{
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();

		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Automatically rotates player to face enemy when close enough to engage in combat.
void AGuardCharacter::InterpToEnemy(float DeltaTime)
{
	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}
}

void AGuardCharacter::DecrementHealth(float Amount)
{
	Health -= Amount;
}

float AGuardCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0;
		Die();

		if (DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);

			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		Health -= DamageAmount;
	}

	DecrementHealth(DamageAmount);

	return DamageAmount;
}

// Gets the enemy closest to the player and assigns it as the combat target.
void AGuardCharacter::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	TSubclassOf<AEnemy> ClassFilter;
	GetOverlappingActors(OverlappingActors, AEnemy::StaticClass());

	if (OverlappingActors.Num() > 0)
	{
		AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);

		if (ClosestEnemy)
		{
			FVector PlayerLocation = GetActorLocation();
			float MinDistance = (ClosestEnemy->GetActorLocation() - PlayerLocation).Size();

			for (auto Actor : OverlappingActors)
			{
				AEnemy* Enemy = Cast<AEnemy>(Actor);

				if (Enemy)
				{
					float DistanceToEnemy = (Enemy->GetActorLocation() - PlayerLocation).Size();

					if (DistanceToEnemy < MinDistance)
					{
						MinDistance = DistanceToEnemy;
						ClosestEnemy = Enemy;
					}
				}
			}

			SetCombatTarget(ClosestEnemy);

			if (MainPlayerController)
			{
				MainPlayerController->DisplayEnemyHealthBar();
			}

			//SetCombatTarget(ClosestEnemy);
			bHasCombatTarget = true;
		}
	}
	else
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemtHealthBar();
		}
	}

}

void AGuardCharacter::Die()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(FName("Death"));
		}

		SetMovementStatus(EMovementStatus::EMS_Dead);
	}
}

void AGuardCharacter::Jump()
{
	if (MainPlayerController)
	{
		if (MainPlayerController->bPauseMenuVisible) return;
	}

	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		Super::Jump();
	}
}

void AGuardCharacter::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AGuardCharacter::IncrementHealth(float Amount)
{
	if ((Health + Amount) >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

void AGuardCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator AGuardCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// Pitch rotation
void AGuardCharacter::LookUp(float Value)
{
	if (CanMove(Value))
	{
		AddControllerPitchInput(Value);
	}
}

void AGuardCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AGuardCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AGuardCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;	
}

//void AGuardCharacter::ShowPickupLocations()
//{
//	for (int32 i = 0; i < PickupLocations.Num(); i++)
//	{
//		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i] + FVector(0.f, 0.f, 75.f), 25.f, 12, FLinearColor::Green, 5.f, 0.5f);
//	}
//}