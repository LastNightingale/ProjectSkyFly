// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyGameStateBase.h"

#include "ProjectDeveloperSettings.h"
#include "SkyFlyGameModeBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/LobbyPlayerController.h"
#include "Player/SkyFlyJetPawn.h"
#include "Instruments/GameInstanceInfo.h"

class UGameInstanceInfo;

void ASkyFlyGameStateBase::BeginPlay()
{
	Super::BeginPlay();	

	ProjectSettings = GetDefault<UProjectDeveloperSettings>();
}

void ASkyFlyGameStateBase::UpdatePlayerList()
{	
	AllPlayerStates = PlayerArray;

	for(int Iter = 0; Iter < PlayerArray.Num(); ++Iter)
	{
		if(ASkyFlyJetPawn* PlayerPawn = PlayerArray[Iter]->GetPawn<ASkyFlyJetPawn>())
		{
			PlayerPawn->HPColor = ProjectSettings->PlayerColors[Iter];
			PlayerPawn->SetHPColor();
		}		
	}
	OnPlayerListChanged.ExecuteIfBound(PlayerArray);

	if(AllPlayerStates.Num() > 1 && HasAuthority())
	{
		auto GI = GetGameInstance<UGameInstanceInfo>();
		GI->SetJoinable(GI->CheckConnectionAmount());
	}
		
}

void ASkyFlyGameStateBase::OnRep_AllPlayerStates()
{
	OnPlayerListChanged.ExecuteIfBound(PlayerArray);
}

void ASkyFlyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyGameStateBase, AllPlayerStates);
}



void ASkyFlyGameStateBase::UpdateControllerWidget(APlayerController* NewPlayer)
{
	//Cast<ALobbyPlayerController>(NewPlayer)->UpdateLobby();
}

void ASkyFlyGameStateBase::CheckStateOfPlayers()
{
	TArray<APlayerState*> Players;

	for(auto Player : PlayerArray)
	{
		if(!Player->GetPawn<ASkyFlyJetPawn>()->bIsGhosted)
			Players.Add(Player);
	}

	if(Players.Num() == 1)
	{
		EndGame();
	}
	
}

void ASkyFlyGameStateBase::EndGame()
{	
	if(HasAuthority())
		GetGameInstance<UGameInstanceInfo>()->ReturnToLobby();
}
