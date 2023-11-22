// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/JoinMenu.h"
#include "Instruments/GameInstanceInfo.h"
#include "Player/MenuPlayerController.h"

void UJoinMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetGameInstance<UGameInstanceInfo>();

	BackButton->OnClicked.AddDynamic(this, &UJoinMenu::OnBackButtonClick);
	JoinButton->OnClicked.AddDynamic(this, &UJoinMenu::OnJoinButtonClick);
}

void UJoinMenu::OnBackButtonClick()
{
	RemoveFromParent();
	this->GetOwningPlayer<AMenuPlayerController>()->ShowMainMenu();
}

void UJoinMenu::OnJoinButtonClick()
{
	GameInstanceRef->JoinLobby("ServerName");
}
