// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/LobbyMenu.h"

#include "Instruments/GameInstanceInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerList = CreateWidget<UPlayerList>(UGameplayStatics::GetPlayerController(GetWorld(),0),
		PlayerListClass);

	StartButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnStartButtonClick);
	LeaveButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnLeaveButtonClick);

	//UpdateMenu();
}

void ULobbyMenu::OnLeaveButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	GetGameInstance<UGameInstanceInfo>()->DestroySession();	
}

void ULobbyMenu::OnStartButtonClick()
{
	//this->RemoveFromParent();
	GetGameInstance<UGameInstanceInfo>()->StartMatch();	
}

void ULobbyMenu::UpdateMenu()
{
	/*if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}*/
}

void ULobbyMenu::SetServer()
{
	StartButton->SetVisibility(ESlateVisibility::Visible);
}
