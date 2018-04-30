// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionInterface.h"
#include "CTFMainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UCTFMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHostButtonClicked();
	UFUNCTION()
	void OnJoinButtonClicked();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnSessionStarted))
	void K2_OnSessionStarted();
	
	void OnSessionStarted(FName SessionName, bool bWasSuccessful);
	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher;

};
