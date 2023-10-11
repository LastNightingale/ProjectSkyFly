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
	DestroySession(); //get rid of it
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
		}
	}
}

void UGameInstanceInfo::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("%d"), Succeeded));
	if(Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/LobbyLevel?listen");
	}
}

void UGameInstanceInfo::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = IsLan;
	SessionSettings.NumPublicConnections = NumberOfPlayers;
	SessionSettings.bAllowJoinInProgress = true;
	SessionInterface->CreateSession(0, NameOfServer, SessionSettings);
}

void UGameInstanceInfo::DestroySession()
{
	SessionInterface->DestroySession(NameOfServer);
}

void UGameInstanceInfo::JoinSession()
{
	
}
