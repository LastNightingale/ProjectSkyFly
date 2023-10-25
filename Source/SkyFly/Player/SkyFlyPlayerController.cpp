// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyPlayerController.h"

#include "EngineUtils.h"
#include "Instruments/SkyFlyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GamePlayerStart.h"

class AGamePlayerStart;

void ASkyFlyPlayerController::Respawn()
{
	if(APawn* OldPlayerPawn = GetPawn())
		OldPlayerPawn->Destroy();
		
	
	APawn* PlayerPawn = GetWorld()->GetAuthGameMode<ASkyFlyGameModeBase>()->SpawnDefaultPawnFor(this,
		FindPlayerStarts());	

	this->Possess(PlayerPawn);
}

AActor* ASkyFlyPlayerController::FindPlayerStarts()
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

void ASkyFlyPlayerController::Client_KickPlayer_Implementation()
{
	//GetGameInstance<UGameInstanceInfo>()->DestroySession();
	//UGameplayStatics::OpenLevel(GetWorld(), "Minimal_Default");
}
