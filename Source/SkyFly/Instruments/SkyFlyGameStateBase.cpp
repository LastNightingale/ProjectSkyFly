// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyGameStateBase.h"

#include "ProjectDeveloperSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/LobbyPlayerController.h"
#include "Player/SkyFlyJetPawn.h"

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

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Was updated: %d"), OnPlayerListChanged.ExecuteIfBound(PlayerArray)));	
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
