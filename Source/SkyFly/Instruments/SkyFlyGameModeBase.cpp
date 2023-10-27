// Copyright Epic Games, Inc. All Rights Reserved.


#include "SkyFlyGameModeBase.h"

#include "EngineUtils.h"
#include "GameInstanceInfo.h"
#include "SkyFlyGameStateBase.h"
#include "Player/SkyFlyJetPawn.h"
#include "Player/SkyFlyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GamePlayerStart.h"

void ASkyFlyGameModeBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ASkyFlyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AllPlayerControllers.Add(Cast<ASkyFlyPlayerController>(NewPlayer));

	Cast<ASkyFlyPlayerController>(NewPlayer)->Respawn();

	UpdatePlayerList();
}

void ASkyFlyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPlayerControllers.Remove(Cast<ASkyFlyPlayerController>(Exiting));

	UpdatePlayerList();
}

void ASkyFlyGameModeBase::KickPlayer(uint8 PlayerID)
{
	AllPlayerControllers[PlayerID]->Client_KickPlayer();
}

void ASkyFlyGameModeBase::UpdatePlayerList()
{	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		GetGameState<ASkyFlyGameStateBase>()->UpdatePlayerList();
	}, 0.1, false);
	
}

void ASkyFlyGameModeBase::EndGame()
{
	//GetGameInstance<UGameInstanceInfo>()->ReturnToLobby();
}
