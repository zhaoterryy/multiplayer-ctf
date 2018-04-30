// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFNameEntryWidget.h"
#include "Button.h"
#include "EditableTextBox.h"
#include "GameFramework/PlayerState.h"
#include "SlateEnums.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTFGameInstance.h"

void UCTFNameEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NicknameInputField->OnTextCommitted.AddDynamic(this, &ThisClass::OnNicknameTextCommitted);
	SubmitButton->OnClicked.AddDynamic(this, &ThisClass::OnSubmitButtonClicked);

	NicknameInputField->SetUserFocus(GetOwningPlayer());
	NicknameInputField->SetKeyboardFocus();

	if (GetWorld())
	{
		UCTFGameInstance* const GI = GetWorld()->GetGameInstanceChecked<UCTFGameInstance>();
		if (!GI->PlayerName.IsEmpty())
		{
			NicknameInputField->SetText(FText::FromString(GI->PlayerName));
		}
	}
}

void UCTFNameEntryWidget::OnNicknameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod != ETextCommit::OnEnter) return;

	if (GetWorld())
	{
		GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->SetPlayerName(Text.ToString());
		RemoveFromParent();
	}
}

void UCTFNameEntryWidget::OnSubmitButtonClicked()
{
	if (GetWorld())
	{
		GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->SetPlayerName(NicknameInputField->Text.ToString());
		RemoveFromParent();
	}
}
