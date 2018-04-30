// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFGameMode.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
#include "CTFShinbiTableRow.h"
#include "CTFCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/GameSession.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"

void ACTFGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ACTFPlayerState* const PlayerState = Cast<ACTFPlayerState>(NewPlayer->PlayerState);

	PlayerState->PlayerNumber = 0;
	PlayerState->SetPlayerName(FString());
	PlayerState->SelectedShinbi = EShinbi::ES_NONE;


	for (uint32 i = 1; i <= 3; i++)
	{
		bool bPlayerNumberIsAvail = true;
		for (APlayerState* Player : GameState->PlayerArray)
		{
			if (Cast<ACTFPlayerState>(Player)->PlayerNumber == i)
			{
				bPlayerNumberIsAvail = false;
			}
		}
		if (bPlayerNumberIsAvail)
		{
			PlayerState->PlayerNumber = i;
			break;
		}
	}

	CastChecked<ACTFPlayerController>(NewPlayer)->OnPostLogin();
}

void ACTFGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ACTFGameMode::StartGame()
{
	StartMatch();
	K2_OnGameBegin();
}

bool ACTFGameMode::ReadyToStartMatch_Implementation()
{
	return false;
}

void ACTFGameMode::InitializeGameLevel()
{
	TArray<FCTFPlayerSpawnsTableRow*> PSArray;
	PlayerSpawnsDataTable->GetAllRows(TEXT(""), PSArray);

	for (APlayerState* Player : GameState->PlayerArray)
	{
		ACTFPlayerState* CTFPlayer = Cast<ACTFPlayerState>(Player);
		int Index = UKismetMathLibrary::RandomInteger(PSArray.Num());
		FTransform PlayerStart = PSArray[Index]->Transform;
		PSArray.RemoveAt(Index);

		TArray<FCTFShinbiTableRow*> Rows;
		ShinbiDataTable->GetAllRows<FCTFShinbiTableRow>(TEXT(""), Rows);
		for (FCTFShinbiTableRow* Row : Rows)
		{
			if (Row->ShinbiType == CTFPlayer->SelectedShinbi)
			{
				ACTFCharacter* Character = GetWorld()->SpawnActor<ACTFCharacter>(ActorToSpawn, PlayerStart);

				GetPlayerControllerFromNetId(GetWorld(), *CTFPlayer->UniqueId)->Possess(Character);
				Character->GetMesh()->SetSkeletalMesh(Row->SkeletalMesh);
				Character->NewSkeletalMesh = Row->SkeletalMesh;
			}
		}
	}

	K2_OnInitGameLevelComplete();
}

void ACTFGameMode::ClearWidgetsOnAllPlayers()
{
	for (APlayerState* Player : GameState->PlayerArray)
	{
		Cast<ACTFPlayerController>(GetPlayerControllerFromNetId(GetWorld(), *Player->UniqueId))->Client_ClearWidgets();
	}
}
