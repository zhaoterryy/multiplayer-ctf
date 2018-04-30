// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerState.h"

UCTFGameInstance::UCTFGameInstance()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCTFGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UCTFGameInstance::OnStartSessionComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCTFGameInstance::OnJoinSessionComplete);
	OnFindSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UCTFGameInstance::OnFindSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UCTFGameInstance::OnDestroySessionComplete);
}

void UCTFGameInstance::HostLobby()
{
#ifdef WITH_EDITOR
	CloseLobby();
#endif
	HostSession(GetFirstLocalPlayerController()->GetLocalPlayer(), "CTF", true, true, 3);
	bIsHost = true;
}

void UCTFGameInstance::JoinLobby()
{
	TSharedPtr<const FUniqueNetId> UserId = GetFirstLocalPlayerController()->GetLocalPlayer()->GetUniqueNetIdFromCachedControllerId();
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();
	
		if (OnlineSession.IsValid() && UserId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true;
			SessionSearch->MaxSearchResults = 1;
			SessionSearch->PingBucketSize = 50;

			OnFindSessionCompleteDelegateHandle = OnlineSession->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegate);
			OnlineSession->FindSessions(*UserId, SessionSearch.ToSharedRef());
		}
	}
}

void UCTFGameInstance::CloseLobby()
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	
	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();

		if (OnlineSession.IsValid())
		{
			OnDestroySessionCompleteDelegateHandle = OnlineSession->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			OnlineSession->DestroySession("CTF");
			GEngine->ShutdownWorldNetDriver(GetWorld());
		}
	}
}

void UCTFGameInstance::SetPlayerName(const FString& S)
{
	GetFirstLocalPlayerController()->PlayerState->SetPlayerName(S);
	PlayerName = S;
}

bool UCTFGameInstance::HostSession(ULocalPlayer* LocalPlayer, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	TSharedPtr<const FUniqueNetId> UserId = LocalPlayer->GetUniqueNetIdFromCachedControllerId();
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();

		if (OnlineSession.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			OnCreateSessionCompleteDelegateHandle = OnlineSession->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return OnlineSession->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

bool UCTFGameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	TSharedPtr<const FUniqueNetId> UserId = LocalPlayer->GetUniqueNetIdFromCachedControllerId();
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();

		if (OnlineSession.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = OnlineSession->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			return OnlineSession->JoinSession(*UserId, "CTF", SearchResult);
		}
	}

	return false;
}

void UCTFGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();

		if (OnlineSession.IsValid())
		{
			OnlineSession->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = OnlineSession->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				OnlineSession->StartSession(SessionName);
			}
		}
	}
}

void UCTFGameInstance::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();
		if (OnlineSession.IsValid())
		{
			OnlineSession->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, start listening
	if (bWasSuccessful)
	{
		FURL Url(nullptr, *GetWorld()->GetLocalURL(), TRAVEL_Absolute);
		GetWorld()->Listen(Url);

		OnSessionStarted.Broadcast(SessionName, bWasSuccessful);
	}
}

void UCTFGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr OnlineSession = OnlineSub->GetSessionInterface();

		if (OnlineSession.IsValid())
		{
			OnlineSession->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && OnlineSession->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);

				OnSessionJoined.Broadcast(SessionName, Result);
			}
		}
	}
}

void UCTFGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful));
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompleteDelegateHandle);

			if (SessionSearch->SearchResults.Num() > 0)
			{
				JoinSession(GetFirstLocalPlayerController()->GetLocalPlayer(), SessionSearch->SearchResults[0]);
			}
		}
	}
}

void UCTFGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		}
	}
}
