// Fill out your copyright notice in the Description page of Project Settings.


#include "Instruments/LobbyGameModeBase.h"

#include "SkyFlyGameStateBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Player/LobbyPlayerController.h"

void ALobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);	
	//Cast<ALobbyPlayerController>(NewPlayer)->BindPlayers();	

	UpdatePlayerList();
	
}

void ALobbyGameModeBase::UpdatePlayerList()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{		
		GetGameState<ASkyFlyGameStateBase>()->UpdatePlayerList();
	}, 0.2, false);
}
