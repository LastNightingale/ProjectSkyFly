// Fill out your copyright notice in the Description page of Project Settings.


#include "Instruments/LobbyGameModeBase.h"

#include "SkyFlyGameStateBase.h"
#include "Player/LobbyPlayerController.h"

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	Cast<ALobbyPlayerController>(NewPlayer)->UpdateLobby();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		
		GetGameState<ASkyFlyGameStateBase>()->UpdatePlayerList();
	}, 0.2, false);
	
}
