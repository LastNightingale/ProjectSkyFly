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

void ASkyFlyGameModeBase::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);

	AllPlayerControllers.Add(Cast<ASkyFlyPlayerController>(C));

	UpdatePlayerList();
}

void ASkyFlyGameModeBase::BeginPlay()
{
	Super::BeginPlay();	
}

void ASkyFlyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AllPlayerControllers.Add(Cast<ASkyFlyPlayerController>(NewPlayer));

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

AActor* ASkyFlyGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	for(TActorIterator<AGamePlayerStart> Iter(GetWorld()); Iter; ++Iter)
	{
		if(!Iter->IsTaken())
		{
			Iter->Take();
			return *Iter;
		}
	}	
	return nullptr;
}
