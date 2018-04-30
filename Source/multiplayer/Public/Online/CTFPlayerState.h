// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CTFPlayerState.generated.h"

UENUM()
enum class EShinbi : uint8
{
	ES_Normal,
	ES_Dynasty,
	ES_Jade,
	ES_NONE
};

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API ACTFPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void ClientInitialize(class AController* C) override;

	UPROPERTY(Replicated)
	uint32 bIsReady : 1;

	UPROPERTY(Replicated)
	uint32 PlayerNumber;

	UPROPERTY(Replicated)
	EShinbi SelectedShinbi;

	void ToggleReady();
	void SetSelectedShinbi(EShinbi Shinbi);

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdatePlayerName(const FString& S);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdateSelectedShinbi(EShinbi Shinbi);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ToggleReady();
};
