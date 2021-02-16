// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemtHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);

		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}

		EnemyHealthBar->SetAlignmentInViewport(FVector2D(0.f, 0.f));
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;

		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);

		PositionInViewport.X -= 70.f;
		PositionInViewport.Y -= 70.f;

		FVector2D SizeInViewport = FVector2D(300.f, 20.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}