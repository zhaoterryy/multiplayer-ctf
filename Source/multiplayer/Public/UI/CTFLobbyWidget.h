// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTFPlayerState.h"
#include "CTFLobbyWidget.generated.h"

class UButton;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UCTFLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintPure)
	FText GetActionButtonText() const;

	UFUNCTION(BlueprintPure)
	FText GetPlayerOneName() const;
	UFUNCTION(BlueprintPure)
	FText GetPlayerTwoName() const;
	UFUNCTION(BlueprintPure)
	FText GetPlayerThreeName() const;

	UFUNCTION(BlueprintPure)
	FLinearColor GetPlayerOneColor() const;
	UFUNCTION(BlueprintPure)
	FLinearColor GetPlayerTwoColor() const;
	UFUNCTION(BlueprintPure)
	FLinearColor GetPlayerThreeColor() const;

	UFUNCTION(BlueprintPure)
	FLinearColor GetPlayerOneMarkerColor() const;
	UFUNCTION(BlueprintPure)
	FLinearColor GetPlayerTwoMarkerColor() const;
	UFUNCTION(BlueprintPure)
	FLinearColor GetPlayerThreeMarkerColor() const;
	
	UFUNCTION(BlueprintPure)
	ESlateVisibility GetShinbiNormButtonVisibility() const;
	UFUNCTION(BlueprintPure)
	ESlateVisibility GetShinbiDynastyButtonVisibility() const;
	UFUNCTION(BlueprintPure)
	ESlateVisibility GetShinbiJadeButtonVisibility() const;

	UFUNCTION(BlueprintPure)
	FLinearColor GetShinbiNormTint() const;
	UFUNCTION(BlueprintPure)
	FLinearColor GetShinbiDynastyTint() const;
	UFUNCTION(BlueprintPure)
	FLinearColor GetShinbiJadeTint() const;

	UFUNCTION(BlueprintPure)
	FLinearColor GetActionButtonColor() const;
	UFUNCTION(BlueprintPure)
	ESlateVisibility GetActionButtonVisibility() const;

	UFUNCTION()
	void OnShinbiNormClicked();
	UFUNCTION()
	void OnShinbiDynastyClicked();
	UFUNCTION()
	void OnShinbiJadeClicked();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnBackButtonClicked))
	void K2_OnBackButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();
	UFUNCTION()
	void OnActionButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher;
	UPROPERTY(meta = (BindWidget))
	UButton* ShinbiNormButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ShinbiDynastyButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ShinbiJadeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ActionButton;

	// helpers
	bool IsEveryoneReady() const;
	FLinearColor GetColorForPlayer(uint32 PlayerNumber) const;
	FText GetNameForPlayer(uint32 PlayerNumber) const;
	FLinearColor GetMarkerColorForPlayer(uint32 PlayerNumber) const;
	bool IsShinbiTaken(EShinbi Shinbi) const;
	ESlateVisibility GetButtonVisibilityForShinbi(EShinbi Shinbi) const;
	FLinearColor GetTintForShinbi(EShinbi Shinbi) const;
};
