// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CTFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API ACTFGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Game Begin"))
	void K2_OnGameBegin();
	
	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	virtual bool ReadyToStartMatch_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void InitializeGameLevel();

	UFUNCTION(BlueprintCallable)
	void ClearWidgetsOnAllPlayers();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Game Initialization Complete"))
	void K2_OnInitGameLevelComplete();

	UPROPERTY(EditAnywhere)
	class UDataTable* ShinbiDataTable;

	UPROPERTY(EditAnywhere)
	class UDataTable* PlayerSpawnsDataTable;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;

};
