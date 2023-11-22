// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceInfo.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "MenuWIdgets/HostMenu.h"
#include "MenuWIdgets/JoinMenu.h"
#include "MenuWIdgets/MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "SkyFlyGameStateBase.h"

void UGameInstanceInfo::LaunchLobby(uint8 PlayerNumber)
{
	NumberOfPlayers = PlayerNumber;
	CreateSession();
}

void UGameInstanceInfo::JoinLobby(const FName& NameServer)
{
	JoinSession();
}

void UGameInstanceInfo::Init()
{
	Super::Init();

	if(IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,
				&UGameInstanceInfo::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
				&UGameInstanceInfo::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,
				&UGameInstanceInfo::OnJoinSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,
				&UGameInstanceInfo::OnDestroySessionComplete);
		}
	}
}

void UGameInstanceInfo::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{	
	if(Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?game=/Game/BPClasses/BP_LobbyGameModeBase.BP_LobbyGameModeBase?listen");
	}
}

void UGameInstanceInfo::OnFindSessionComplete(bool Succeeded)
{
	if(Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		if(SearchResults.Num())
		{
			SessionInterface->JoinSession(0, TEXT("ServerName"), SearchResults[0]);			
		}
	}
}

void UGameInstanceInfo::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{	
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);		
		
		if(JoinAddress != "")
			PC->ClientTravel(JoinAddress, TRAVEL_Absolute);
	}
}

void UGameInstanceInfo::OnDestroySessionComplete(FName ServerName, bool Succeeded)
{	
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void UGameInstanceInfo::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = NumberOfPlayers;
	SessionSettings.NumPrivateConnections = 0;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionInterface->CreateSession(0, TEXT("ServerName"), SessionSettings);
}

void UGameInstanceInfo::DestroySession()
{
	SessionInterface->DestroySession(TEXT("ServerName"));	
}

void UGameInstanceInfo::JoinSession()
{	
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 200000; 
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UGameInstanceInfo::StartMatch()
{		
	GetWorld()->ServerTravel("/Game/Maps/LevelMap?game="
						  "/Game/BPClasses/BP_SkyFlyGameModeBase.BP_SkyFlyGameModeBase?listen");
}
void UGameInstanceInfo::ReturnToLobby()
{
	GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?game=/Game/BPClasses/BP_LobbyGameModeBase.BP_LobbyGameModeBase?listen", true);
}
