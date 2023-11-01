// Fill out your copyright notice in the Description page of Project Settings.


#include "Instruments/LobbyGameModeBase.h"

#include "SkyFlyGameStateBase.h"
#include "Player/LobbyPlayerController.h"

void ALobbyGameModeBase::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);

	AllPlayerControllers.Add(Cast<ALobbyPlayerController>(C));

	UpdatePlayerList();	
}

void ALobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPlayerControllers.Remove(Cast<ALobbyPlayerController>(Exiting));

	UpdatePlayerList();
}

void ALobbyGameModeBase::UpdatePlayerList()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{		
		GetGameState<ASkyFlyGameStateBase>()->UpdatePlayerList();
	}, 0.2, false);
}
