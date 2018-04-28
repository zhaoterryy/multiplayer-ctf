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
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnPossess(APawn* PossessedPawn);
};
