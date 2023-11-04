// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/LobbyMenu.h"

#include "GameFramework/GameStateBase.h"
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
}

void ULobbyMenu::OnLeaveButtonClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	GetGameInstance<UGameInstanceInfo>()->DestroySession();	
}

void ULobbyMenu::OnStartButtonClick()
{
	if(GetWorld()->GetGameState()->PlayerArray.Num() > 1)
		GetGameInstance<UGameInstanceInfo>()->StartMatch();	
}

void ULobbyMenu::SetServer()
{
	StartButton->SetVisibility(ESlateVisibility::Visible);
}
