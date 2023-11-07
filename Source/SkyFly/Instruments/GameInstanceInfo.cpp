// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceInfo.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MenuWIdgets/HostMenu.h"
#include "MenuWIdgets/JoinMenu.h"
#include "MenuWIdgets/MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "SkyFlyGameStateBase.h"

void UGameInstanceInfo::ShowMainMenu()
{
	if(!MainMenu)
	{
		if(!MainMenuClass)
			return;
		
		MainMenu = CreateWidget<UMainMenu>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			MainMenuClass);
	}

	if(!MainMenu->IsInViewport())
	MainMenu->AddToViewport();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(GetWorld(), 0),
		MainMenu);
}

void UGameInstanceInfo::ShowHostMenu()
{
	if(!HostMenu)
	{
		if(!HostMenuClass)
			return;
		
		HostMenu = CreateWidget<UHostMenu>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			HostMenuClass);
	}

	if(!HostMenu->IsInViewport())
	HostMenu->AddToViewport();
}

void UGameInstanceInfo::ShowJoinMenu()
{
	if(!JoinMenuClass)
		return;
		
	CreateWidget<UJoinMenu>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
		JoinMenuClass)->AddToViewport();
	
}

void UGameInstanceInfo::LaunchLobby(uint8 PlayerNumber, bool LAN, const FName& NameServer)
{
	NumberOfPlayers = PlayerNumber;
	NameOfServer = NameServer;
	IsLan = LAN;
	CreateSession();
}

void UGameInstanceInfo::JoinLobby(const FName& NameServer)
{
	NameOfServerToJoin = NameServer;
	JoinSession();
}

void UGameInstanceInfo::SetSessionName(const FName Name)
{
	NameOfServer = Name;
}

FName UGameInstanceInfo::GetSessionName() const
{
	return NameOfServer;
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
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Created: %d"), Succeeded));*/
	if(Succeeded)
	{
		//GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?listen");
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
			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("%d"), SearchResults[0].Session.SessionSettings.bShouldAdvertise));*/
			//if(SearchResults[0].Session.SessionSettings.bShouldAdvertise)
			SessionInterface->JoinSession(0, "ServerName", SearchResults[0]);			
		}
	}
}

void UGameInstanceInfo::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		FString::Printf(TEXT("Joined: %s"), *JoinAddress));*/
		if(JoinAddress != "")
			PC->ClientTravel(JoinAddress, TRAVEL_Absolute);
	}
}

void UGameInstanceInfo::OnDestroySessionComplete(FName ServerName, bool Succeeded)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		FString::Printf(TEXT("Destroyed session: %d"), Succeeded));*/
	UGameplayStatics::OpenLevel(GetWorld(), "Main Menu");
}

void UGameInstanceInfo::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.NumPublicConnections = NumberOfPlayers;
	SessionSettings.NumPrivateConnections = 0;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("%s"), *NameOfServer.ToString()));*/
	//SessionInterface->CreateSession(0, NameOfServer, SessionSettings);
	SessionInterface->CreateSession(0, "ServerName", SessionSettings);
}

void UGameInstanceInfo::DestroySession()
{
	SessionInterface->DestroySession("ServerName");	
}

void UGameInstanceInfo::JoinSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	//SessionSearch->bIsLanQuery = true;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 150000; 
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//SessionSearch->QuerySettings.Set(FName("SESSION_NAME"), NameOfServerToJoin, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UGameInstanceInfo::StartMatch()
{
	//GetWorld()->ServerTravel("/Game/StarterContent/Maps/Minimal_Default?listen");
	SetJoinable(false);
	if(SessionInterface)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("%d, %d, %d"), SessionInterface->GetSessionSettings(NameOfServer)->bAllowJoinInProgress,
			SessionInterface->GetSessionSettings(NameOfServer)->bShouldAdvertise,
			SessionInterface->GetSessionSettings(NameOfServer)->bUsesPresence));*/
	}
	GetWorld()->ServerTravel("/Game/StarterContent/Maps/Minimal_Default?game="
						  "/Game/BPClasses/BP_SkyFlyGameModeBase.BP_SkyFlyGameModeBase?listen", true);
}

void UGameInstanceInfo::ReturnToLobby()
{
	//GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?listen");
	//GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?game=/Game/BPClasses/BP_LobbyGameModeBase.BP_LobbyGameModeBase?listen", false, false);
	SetJoinable(CheckConnectionAmount());
	GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?listen", true);
}

void UGameInstanceInfo::SetJoinable(bool bIsJoinable)
{
	/*if(SessionInterface)
	{
		SessionInterface->GetSessionSettings("ServerName")->bAllowJoinInProgress = bIsJoinable;
		SessionInterface->GetSessionSettings("ServerName")->bShouldAdvertise = bIsJoinable;
		SessionInterface->GetSessionSettings("ServerName")->bUsesPresence = bIsJoinable;
	}*/
}

bool UGameInstanceInfo::CheckConnectionAmount()
{
	return GetWorld()->GetGameState<ASkyFlyGameStateBase>()->AllPlayerStates.Num() < NumberOfPlayers;
}
