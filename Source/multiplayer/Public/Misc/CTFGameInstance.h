// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionInterface.h"
#include "CTFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYER_API UCTFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCTFGameInstance();

	void HostLobby();
	void JoinLobby();
	void CloseLobby();
	void SetPlayerName(const FString& S);

	FOnStartSessionComplete OnSessionStarted;
	FOnJoinSessionComplete OnSessionJoined;

protected:
	virtual bool HostSession(ULocalPlayer* LocalPlayer, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnFindSessionComplete(bool bWasSuccessful);
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionCompleteDelegate;

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

public:
	FString PlayerName;
	uint32 bIsHost : 1;
};
