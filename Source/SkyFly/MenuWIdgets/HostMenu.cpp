// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/HostMenu.h"
#include "Instruments/GameInstanceInfo.h"

void UHostMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetGameInstance<UGameInstanceInfo>();

	CreateButton->SetIsEnabled(false);
	
	BackButton->OnClicked.AddDynamic(this, &UHostMenu::OnBackButtonClick);
	CreateButton->OnClicked.AddDynamic(this, &UHostMenu::OnCreateButtonClick);
	ServerName->OnTextChanged.AddDynamic(this, &UHostMenu::OnServerNameChanged);
}

void UHostMenu::OnBackButtonClick()
{
	RemoveFromParent();
	GameInstanceRef->ShowMainMenu();
}

void UHostMenu::OnCreateButtonClick()
{
	GameInstanceRef->LaunchLobby(2, true, FName("Lobby"));
}

void UHostMenu::OnServerNameChanged(const FText& Text)
{
	Text.IsEmpty() ? CreateButton->SetIsEnabled(false) : CreateButton->SetIsEnabled(true);
}
