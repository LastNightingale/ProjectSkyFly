// Fill out your copyright notice in the Description page of Project Settings.


#include "Instruments/LobbyGameModeBase.h"

#include "SkyFlyGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/LobbyPlayerController.h"

void ALobbyGameModeBase::GenericPlayerInitialization(AController* C)
{
	Super::GenericPlayerInitialization(C);

	AllPlayerControllers.Add(Cast<ALobbyPlayerController>(C));

	UpdatePlayerList();	
}

void ALobbyGameModeBase::Logout(AController* Exiting)
{
	/*AllPlayerControllers.Remove(Cast<ALobbyPlayerController>(Exiting));

	UpdatePlayerList();*/
	
	//auto PC = Cast<APlayerController>(Exiting);
	if(!Cast<APlayerController>(Exiting)->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Done"));
		AllPlayerControllers.Remove(Cast<ALobbyPlayerController>(Exiting));

		UpdatePlayerList();
	}
	
	Super::Logout(Exiting);	
}

void ALobbyGameModeBase::KickPlayer(uint8 PlayerID)
{
	AllPlayerControllers[PlayerID]->Client_KickPlayer();
}

void ALobbyGameModeBase::UpdatePlayerList()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		if(auto GS = GetGameState<ASkyFlyGameStateBase>())
			GS->UpdatePlayerList();
	}, 0.1, false);
	//GetGameState<ASkyFlyGameStateBase>()->UpdatePlayerList();
}

void ALobbyGameModeBase::ResetTimer()
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle))
		GetWorldTimerManager().ClearTimer(TimerHandle);
}
