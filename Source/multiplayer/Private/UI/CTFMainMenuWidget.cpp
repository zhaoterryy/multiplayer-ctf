// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFMainMenuWidget.h"
#include "Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTFGameInstance.h"
#include "WidgetSwitcher.h"

void UCTFMainMenuWidget::NativeConstruct()
{
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);

	Super::NativeConstruct();
}

void UCTFMainMenuWidget::OnHostButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, (FString)L"HOST");
	Switcher->SetActiveWidgetIndex(2);

	if (UCTFGameInstance* GameInstance = Cast<UCTFGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->OnSessionStarted.AddUObject(this, &ThisClass::OnSessionStarted);
		GameInstance->HostLobby();
	}
}

void UCTFMainMenuWidget::OnJoinButtonClicked()
{
	UKismetSystemLibrary::PrintString(this, (FString)L"JOIN");
	Switcher->SetActiveWidgetIndex(2);

	if (GetWorld())
	{
		if (UCTFGameInstance* GameInstance = Cast<UCTFGameInstance>(GetWorld()->GetGameInstance()))
		{
			GameInstance->OnSessionJoined.AddUObject(this, &ThisClass::OnSessionJoined);
			GameInstance->JoinLobby();
		}
	}
}

void UCTFMainMenuWidget::OnSessionStarted(FName SessionName, bool bWasSuccessful)
{
	GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->OnSessionStarted.RemoveAll(this);
	K2_OnSessionStarted();
}

void UCTFMainMenuWidget::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	RemoveFromParent();
}
