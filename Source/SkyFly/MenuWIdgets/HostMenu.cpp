// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/HostMenu.h"
#include "Instruments/GameInstanceInfo.h"
#include "Player/MenuPlayerController.h"

void UHostMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetGameInstance<UGameInstanceInfo>();

	//CreateButton->SetIsEnabled(false);

	NumberOfPlayers = 2;
	MaxNumberOfPlayers = 4;
	EnabledLAN = true;
	NetText = FText::FromString("LAN");
	
	BackButton->OnClicked.AddDynamic(this, &UHostMenu::OnBackButtonClick);
	CreateButton->OnClicked.AddDynamic(this, &UHostMenu::OnCreateButtonClick);
	//ServerName->OnTextChanged.AddDynamic(this, &UHostMenu::OnServerNameChanged);
	IncreasePlayersButton->OnClicked.AddDynamic(this, &UHostMenu::OnIncreasePlayersButtonClick);
	DecreasePlayersButton->OnClicked.AddDynamic(this, &UHostMenu::OnDecreasePlayersButtonClick);
	/*IncreaseNetButton->OnClicked.AddDynamic(this, &UHostMenu::OnIncreaseNetButtonClick);
	DecreaseNetButton->OnClicked.AddDynamic(this, &UHostMenu::OnDecreaseNetButtonClick);*/
}

void UHostMenu::OnBackButtonClick()
{
	RemoveFromParent();
	//GameInstanceRef->ShowMainMenu();
	this->GetOwningPlayer<AMenuPlayerController>()->ShowMainMenu();
}

void UHostMenu::OnCreateButtonClick()
{
	GameInstanceRef->LaunchLobby(4, false, "ServerName");
}

void UHostMenu::OnDecreasePlayersButtonClick()
{
	NumberOfPlayers = FMath::Clamp<uint8>(NumberOfPlayers - 1, 2, MaxNumberOfPlayers);
}

void UHostMenu::OnIncreasePlayersButtonClick()
{
	NumberOfPlayers = FMath::Clamp<uint8>(NumberOfPlayers + 1, 2, MaxNumberOfPlayers);
}

void UHostMenu::OnDecreaseNetButtonClick()
{
	if(!EnabledLAN)
	{
		EnabledLAN = true;
		NetText = FText::FromString("LAN");
	}
}

void UHostMenu::OnIncreaseNetButtonClick()
{
	if(EnabledLAN)
	{
		EnabledLAN = false;
		NetText = FText::FromString("INTERNET");
	}
}

void UHostMenu::OnServerNameChanged(const FText& Text)
{
	Text.IsEmpty() ? CreateButton->SetIsEnabled(false) : CreateButton->SetIsEnabled(true);
}
