// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFPlayerController.h"




void ACTFPlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	OnPossess(aPawn);
}
