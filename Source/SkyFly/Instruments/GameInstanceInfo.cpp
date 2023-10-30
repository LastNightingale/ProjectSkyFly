// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceInfo.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MenuWIdgets/HostMenu.h"
#include "MenuWIdgets/JoinMenu.h"
#include "MenuWIdgets/MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

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
	ShowLoadingScreen();
	CreateSession();
	//DestroySession(); //get rid of it
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

void UGameInstanceInfo::ShowLoadingScreen()
{
	
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
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
		FString::Printf(TEXT("Found: %d"), Succeeded));*/
	if(Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,
		FString::Printf(TEXT("How many found: %d"), SearchResults.Num()));*/
		if(SearchResults.Num())
		{		
			SessionInterface->JoinSession(0, NameOfServerToJoin, SearchResults[0]);			
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

void UGameInstanceInfo::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bIsLANMatch = IsLan;
	SessionSettings.NumPublicConnections = NumberOfPlayers;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.Settings.Add(FName("SERVER_NAME"), NameOfServer.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Added: %s"), *NameOfServer.ToString()));
	SessionInterface->CreateSession(0, NameOfServer, SessionSettings);
}

void UGameInstanceInfo::DestroySession()
{
	SessionInterface->DestroySession(NameOfServer);	
}

void UGameInstanceInfo::JoinSession()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true; //LAN only, change later
	SessionSearch->MaxSearchResults = 100; 
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//SessionSearch->QuerySettings.Set(FName("SESSION_NAME"), NameOfServerToJoin, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UGameInstanceInfo::StartMatch()
{
	//GetWorld()->ServerTravel("/Game/StarterContent/Maps/Minimal_Default?listen");
	GetWorld()->ServerTravel("/Game/StarterContent/Maps/Minimal_Default?game="
						  "/Game/BPClasses/BP_SkyFlyGameModeBase.BP_SkyFlyGameModeBase?listen");
}

void UGameInstanceInfo::ReturnToLobby()
{
	//GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?listen");
	//GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?game=/Game/BPClasses/BP_LobbyGameModeBase.BP_LobbyGameModeBase?listen", false, false);
	GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?listen", true);
	//UGameplayStatics::OpenLevel(GetWorld(), "LobbyLevel", true, "listen");
}
