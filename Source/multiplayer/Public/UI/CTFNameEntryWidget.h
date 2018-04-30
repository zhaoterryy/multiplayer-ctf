// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTFNameEntryWidget.generated.h"

class UEditableTextBox;
class UButton;

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UCTFNameEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnNicknameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnSubmitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* NicknameInputField;
	UPROPERTY(meta = (BindWidget))
	UButton* SubmitButton;

};
