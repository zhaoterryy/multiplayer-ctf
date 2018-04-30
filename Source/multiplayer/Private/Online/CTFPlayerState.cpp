// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFPlayerState.h"
#include "UnrealNetwork.h"
#include "CTFGameInstance.h"

void ACTFPlayerState::ClientInitialize(class AController* C)
{
	Super::ClientInitialize(C);

	if (GetNetMode() == NM_Client)
	{
		Server_UpdatePlayerName(GetWorld()->GetGameInstanceChecked<UCTFGameInstance>()->PlayerName);
	}
}

void ACTFPlayerState::ToggleReady()
{
	Server_ToggleReady();
}

void ACTFPlayerState::SetSelectedShinbi(EShinbi Shinbi)
{
	Server_UpdateSelectedShinbi(Shinbi);
}

void ACTFPlayerState::Server_UpdateSelectedShinbi_Implementation(EShinbi Shinbi)
{
	SelectedShinbi = Shinbi;
	ForceNetUpdate();
}

bool ACTFPlayerState::Server_UpdateSelectedShinbi_Validate(EShinbi Shinbi)
{
	return true;
}

void ACTFPlayerState::Server_ToggleReady_Implementation()
{
	bIsReady = !bIsReady;
	ForceNetUpdate();
}

bool ACTFPlayerState::Server_ToggleReady_Validate()
{
	return true;
}

void ACTFPlayerState::Server_UpdatePlayerName_Implementation(const FString& S)
{
	SetPlayerName(S);
	ForceNetUpdate();
}

bool ACTFPlayerState::Server_UpdatePlayerName_Validate(const FString& S)
{
	return true;
}

void ACTFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTFPlayerState, bIsReady);
	DOREPLIFETIME(ACTFPlayerState, PlayerNumber);
	DOREPLIFETIME(ACTFPlayerState, SelectedShinbi);
}
