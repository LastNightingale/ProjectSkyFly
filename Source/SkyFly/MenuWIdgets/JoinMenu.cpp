// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/JoinMenu.h"
#include "Instruments/GameInstanceInfo.h"
#include "Player/MenuPlayerController.h"

void UJoinMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetGameInstance<UGameInstanceInfo>();

	//JoinButton->SetIsEnabled(false);

	BackButton->OnClicked.AddDynamic(this, &UJoinMenu::OnBackButtonClick);
	JoinButton->OnClicked.AddDynamic(this, &UJoinMenu::OnJoinButtonClick);
	//ServerName->OnTextChanged.AddDynamic(this, &UJoinMenu::OnServerNameChanged);
}

void UJoinMenu::OnBackButtonClick()
{
	RemoveFromParent();
	//GameInstanceRef->ShowMainMenu();
	this->GetOwningPlayer<AMenuPlayerController>()->ShowMainMenu();
}

void UJoinMenu::OnJoinButtonClick()
{
	//GameInstanceRef->JoinLobby(FName(*ServerName->Text.ToString()));
	GameInstanceRef->JoinLobby("ServerName");
}

void UJoinMenu::OnServerNameChanged(const FText& Text)
{
	Text.IsEmpty() ? JoinButton->SetIsEnabled(false) : JoinButton->SetIsEnabled(true);
}
