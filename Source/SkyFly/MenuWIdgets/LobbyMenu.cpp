// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/LobbyMenu.h"

#include "GameFramework/GameStateBase.h"
#include "Instruments/GameInstanceInfo.h"
#include "Instruments/LobbyGameModeBase.h"
#include "Instruments/SkyFlyGameStateBase.h"
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
	if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		for(int Iter = GetWorld()->GetGameState<ASkyFlyGameStateBase>()->AllPlayerStates.Num() - 1; Iter > 0; Iter--)
		{
			GetWorld()->GetAuthGameMode<ALobbyGameModeBase>()->KickPlayer(Iter);
		}			
	}
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
