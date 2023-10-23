// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyGameStateBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/LobbyPlayerController.h"

void ASkyFlyGameStateBase::UpdatePlayerList()
{	
	AllPlayerStates = PlayerArray;	

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Was updated: %d"), OnPlayerListChanged.ExecuteIfBound(PlayerArray)));	
}

void ASkyFlyGameStateBase::UpdatePlayerListGame()
{
	
}

void ASkyFlyGameStateBase::OnRep_AllPlayerStates()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Was updated client: %d"),
			OnPlayerListChanged.ExecuteIfBound(PlayerArray)));
}

void ASkyFlyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyGameStateBase, AllPlayerStates);
}



void ASkyFlyGameStateBase::UpdateControllerWidget(APlayerController* NewPlayer)
{
	Cast<ALobbyPlayerController>(NewPlayer)->UpdateLobby();
}
