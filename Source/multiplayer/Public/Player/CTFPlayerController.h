// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CTFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API ACTFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Possess(APawn* aPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void OnPostLogin();

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_ClearWidgets();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable, WithValidation)
	void Client_OnPostLogin();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFirstEntry();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLobbyJoined();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossess(APawn* PossessedPawn);

	void ForwardPressed();
	void ForwardReleased();

	float DashTimer;

private:
	class ACTFCharacter* GetCTFCharacter();
	class ACTFCharacter* CTFCharacter;
};
