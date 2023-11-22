// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/HostMenu.h"
#include "Instruments/GameInstanceInfo.h"
#include "Player/MenuPlayerController.h"

void UHostMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetGameInstance<UGameInstanceInfo>();

	NumberOfPlayers = 2;
	MaxNumberOfPlayers = 4;
	
	BackButton->OnClicked.AddDynamic(this, &UHostMenu::OnBackButtonClick);
	CreateButton->OnClicked.AddDynamic(this, &UHostMenu::OnCreateButtonClick);
	IncreasePlayersButton->OnClicked.AddDynamic(this, &UHostMenu::OnIncreasePlayersButtonClick);
	DecreasePlayersButton->OnClicked.AddDynamic(this, &UHostMenu::OnDecreasePlayersButtonClick);
}

void UHostMenu::OnBackButtonClick()
{
	RemoveFromParent();
	this->GetOwningPlayer<AMenuPlayerController>()->ShowMainMenu();
}

void UHostMenu::OnCreateButtonClick()
{
	GameInstanceRef->LaunchLobby(NumberOfPlayers);
}

void UHostMenu::OnDecreasePlayersButtonClick()
{
	NumberOfPlayers = FMath::Clamp<uint8>(NumberOfPlayers - 1, 2, MaxNumberOfPlayers);
}

void UHostMenu::OnIncreasePlayersButtonClick()
{
	NumberOfPlayers = FMath::Clamp<uint8>(NumberOfPlayers + 1, 2, MaxNumberOfPlayers);
}
