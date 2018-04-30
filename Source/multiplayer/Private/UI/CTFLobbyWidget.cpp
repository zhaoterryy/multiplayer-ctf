// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFLobbyWidget.h"
#include "Button.h"
#include "CTFGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameStateBase.h"
#include "CTFGameMode.h"
#include "WidgetSwitcher.h"

void UCTFLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	ActionButton->OnClicked.AddDynamic(this, &ThisClass::OnActionButtonClicked);
	ShinbiNormButton->OnClicked.AddDynamic(this, &ThisClass::OnShinbiNormClicked);
	ShinbiDynastyButton->OnClicked.AddDynamic(this, &ThisClass::OnShinbiDynastyClicked);
	ShinbiJadeButton->OnClicked.AddDynamic(this, &ThisClass::OnShinbiJadeClicked);
}

void UCTFLobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		if (GetWorld()->GetGameState()->HasMatchStarted())
		{
			Switcher->SetActiveWidgetIndex(1);
		}
	}
}

FText UCTFLobbyWidget::GetActionButtonText() const
{
	if (GetWorld() && GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->bIsHost && IsEveryoneReady())
	{
		return NSLOCTEXT("CTF", "Start", "START");
	}

	if (GetOwningPlayerState<ACTFPlayerState>())
	{
		if (GetOwningPlayerState<ACTFPlayerState>(true)->bIsReady)
		{
			return NSLOCTEXT("CTF", "Unready", "UNREADY");
		}
		else
		{
			return NSLOCTEXT("CTF", "Ready", "READY");
		}
	}

	return FText::GetEmpty();
}

FText UCTFLobbyWidget::GetPlayerOneName() const
{
	return GetNameForPlayer(1);
}

FText UCTFLobbyWidget::GetPlayerTwoName() const
{
	return GetNameForPlayer(2);
}

FText UCTFLobbyWidget::GetPlayerThreeName() const
{
	return GetNameForPlayer(3);
}

FLinearColor UCTFLobbyWidget::GetPlayerOneColor() const
{
	return GetColorForPlayer(1);
}

FLinearColor UCTFLobbyWidget::GetPlayerTwoColor() const
{
	return GetColorForPlayer(2);
}

FLinearColor UCTFLobbyWidget::GetPlayerThreeColor() const
{
	return GetColorForPlayer(3);
}

FLinearColor UCTFLobbyWidget::GetPlayerOneMarkerColor() const
{
	return GetMarkerColorForPlayer(1);
}

FLinearColor UCTFLobbyWidget::GetPlayerTwoMarkerColor() const
{
	return GetMarkerColorForPlayer(2);
}

FLinearColor UCTFLobbyWidget::GetPlayerThreeMarkerColor() const
{
	return GetMarkerColorForPlayer(3);
}

ESlateVisibility UCTFLobbyWidget::GetShinbiNormButtonVisibility() const
{
	return GetButtonVisibilityForShinbi(EShinbi::ES_Normal);
}

ESlateVisibility UCTFLobbyWidget::GetShinbiDynastyButtonVisibility() const
{
	return GetButtonVisibilityForShinbi(EShinbi::ES_Dynasty);
}

ESlateVisibility UCTFLobbyWidget::GetShinbiJadeButtonVisibility() const
{
	return GetButtonVisibilityForShinbi(EShinbi::ES_Jade);
}

FLinearColor UCTFLobbyWidget::GetShinbiNormTint() const
{
	return GetTintForShinbi(EShinbi::ES_Normal);
}

FLinearColor UCTFLobbyWidget::GetShinbiDynastyTint() const
{
	return GetTintForShinbi(EShinbi::ES_Dynasty);
}

FLinearColor UCTFLobbyWidget::GetShinbiJadeTint() const
{
	return GetTintForShinbi(EShinbi::ES_Jade);
}

FLinearColor UCTFLobbyWidget::GetActionButtonColor() const
{
	if (GetActionButtonVisibility() == ESlateVisibility::HitTestInvisible)
	{
		return FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}
	return FLinearColor::White;
}

ESlateVisibility UCTFLobbyWidget::GetActionButtonVisibility() const
{
	if (GetOwningPlayerState())
	{
		if (GetOwningPlayerState<ACTFPlayerState>(true)->SelectedShinbi == EShinbi::ES_NONE)
		{
			return ESlateVisibility::HitTestInvisible;
		}
	}
	return ESlateVisibility::Visible;
}

void UCTFLobbyWidget::OnShinbiNormClicked()
{
	GetOwningPlayerState<ACTFPlayerState>(true)->SetSelectedShinbi(EShinbi::ES_Normal);
}

void UCTFLobbyWidget::OnShinbiDynastyClicked()
{
	GetOwningPlayerState<ACTFPlayerState>(true)->SetSelectedShinbi(EShinbi::ES_Dynasty);
}

void UCTFLobbyWidget::OnShinbiJadeClicked()
{
	GetOwningPlayerState<ACTFPlayerState>(true)->SetSelectedShinbi(EShinbi::ES_Jade);
}

void UCTFLobbyWidget::OnBackButtonClicked()
{
	if (GetWorld())
	{
		UCTFGameInstance* const GI = GetWorld()->GetGameInstanceChecked<UCTFGameInstance>();
		if (GI->bIsHost)
		{
			GI->CloseLobby();
		}
	}
	K2_OnBackButtonClicked();
}

void UCTFLobbyWidget::OnActionButtonClicked()
{
	if (GetWorld() && GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->bIsHost && IsEveryoneReady())
	{
		GetWorld()->GetAuthGameMode<ACTFGameMode>()->StartGame();
		Switcher->SetActiveWidgetIndex(1);
	}
	else
	{
		GetOwningPlayerState<ACTFPlayerState>(true)->ToggleReady();
	}
}

bool UCTFLobbyWidget::IsEveryoneReady() const
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		TArray<APlayerState*>& PlayerArray = GetWorld()->GetGameState()->PlayerArray;
		if (PlayerArray.Num() == 3)
		{
			for (APlayerState* Player : PlayerArray)
			{
				if (!Cast<ACTFPlayerState>(Player)->bIsReady)
				{
					return false;
				}
			}
			return true;
		}
	}

	return false;
}

FLinearColor UCTFLobbyWidget::GetColorForPlayer(uint32 PlayerNumber) const
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		TArray<APlayerState*>& PlayerArray = GetWorld()->GetGameState()->PlayerArray;
		for (APlayerState* Player : PlayerArray)
		{
			ACTFPlayerState* CTFPlayer = Cast<ACTFPlayerState>(Player);
			if (CTFPlayer->PlayerNumber == PlayerNumber)
			{
				if (CTFPlayer->bIsReady)
				{
					return FLinearColor(0.05f, 0.35f, 0.05f, 1.0f);
				}
			}
		}
	}
	return FLinearColor(0.35f, 0.05f, 0.05f, 1.0f);
}

FText UCTFLobbyWidget::GetNameForPlayer(uint32 PlayerNumber) const
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		TArray<APlayerState*>& PlayerArray = GetWorld()->GetGameState()->PlayerArray;
		for (APlayerState* Player : PlayerArray)
		{
			if (Cast<ACTFPlayerState>(Player)->PlayerNumber == PlayerNumber)
			{
				return FText::FromString(Player->GetPlayerName());
			}
		}
	}
	return NSLOCTEXT("CTF", "Waiting for player", "Waiting for player...");
}

FLinearColor UCTFLobbyWidget::GetMarkerColorForPlayer(uint32 PlayerNumber) const
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		TArray<APlayerState*>& PlayerArray = GetWorld()->GetGameState()->PlayerArray;
		for (APlayerState* Player : PlayerArray)
		{
			ACTFPlayerState* CTFPlayer = Cast<ACTFPlayerState>(Player);
			if (CTFPlayer->PlayerNumber == PlayerNumber)
			{
				switch (CTFPlayer->SelectedShinbi)
				{
					case EShinbi::ES_Normal:
						return FLinearColor(0.55f, 0.25f, 0.6f, 1.0f);
					case EShinbi::ES_Dynasty:
						return FLinearColor(1.0f, 0.65f, 0.25f, 1.0f);
					case EShinbi::ES_Jade:
						return FLinearColor(0.0f, 1.0f, 0.26f, 1.0f);
				}
			}
		}
	}
	return FLinearColor::Transparent;
}

bool UCTFLobbyWidget::IsShinbiTaken(EShinbi Shinbi) const
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		TArray<APlayerState*>& PlayerArray = GetWorld()->GetGameState()->PlayerArray;
		for (APlayerState* Player : PlayerArray)
		{
			ACTFPlayerState* CTFPlayer = Cast<ACTFPlayerState>(Player);
			if (CTFPlayer->SelectedShinbi == Shinbi)
			{
				if (CTFPlayer->bIsReady)
				{
					return true;
				}
			}
		}
	}
	return false;
}

ESlateVisibility UCTFLobbyWidget::GetButtonVisibilityForShinbi(EShinbi Shinbi) const
{
	if (GetOwningPlayerState() && GetOwningPlayerState<ACTFPlayerState>(true)->bIsReady || IsShinbiTaken(Shinbi))
	{
		return ESlateVisibility::HitTestInvisible;
	}

	return ESlateVisibility::Visible;
}

FLinearColor UCTFLobbyWidget::GetTintForShinbi(EShinbi Shinbi) const
{
	if (IsShinbiTaken(Shinbi))
	{
		return FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}
	
	return FLinearColor::White;
}
