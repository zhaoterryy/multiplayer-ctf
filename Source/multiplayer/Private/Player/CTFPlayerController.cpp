// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFPlayerController.h"
#include "CTFGameInstance.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "WidgetLayoutLibrary.h"
#include "CTFCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void ACTFPlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	CTFCharacter = CastChecked<ACTFCharacter>(aPawn);
	OnPossess(aPawn);
}

void ACTFPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DashTimer += DeltaSeconds;
}

void ACTFPlayerController::OnPostLogin()
{
	switch (GetNetMode())
	{
		case NM_Standalone: return;
		case NM_ListenServer: Client_OnPostLogin();
	}
}

void ACTFPlayerController::Client_ClearWidgets_Implementation()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

bool ACTFPlayerController::Client_ClearWidgets_Validate()
{
	return true;
}

void ACTFPlayerController::Client_OnPostLogin_Implementation()
{
	OnLobbyJoined();
}

bool ACTFPlayerController::Client_OnPostLogin_Validate()
{
	return true;
}

void ACTFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->PlayerName.IsEmpty())
	{
		OnFirstEntry();
	}
}

void ACTFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Forward", IE_Pressed, this, &ThisClass::ForwardPressed).bConsumeInput = false;
}

void ACTFPlayerController::ForwardPressed()
{
	if (DashTimer <= 0.2f)
	{
		if (GetCTFCharacter())
		{
			GetCTFCharacter()->Dash();
		}
	}

	DashTimer = 0.0f;
}

ACTFCharacter* ACTFPlayerController::GetCTFCharacter()
{
	if (!CTFCharacter)
	{
		CTFCharacter = Cast<ACTFCharacter>(GetPawn());
	}
	return CTFCharacter;
}
