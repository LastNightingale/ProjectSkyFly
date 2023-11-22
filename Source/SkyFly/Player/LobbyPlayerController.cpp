// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Instruments/GameInstanceInfo.h"
#include "Instruments/SkyFlyGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerController::UpdateLobby()
{	
	if(!IsLocalController())
	{
		ClientUpdateLobby();
		return;
	}
	
	if(!LobbyMenu)
	{		
		if(!LobbyMenuClass)
			return;
		
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);
		if(HasAuthority())
			LobbyMenu->SetServer();

		GameStateRef->OnPlayerListChanged.Unbind();
		GameStateRef->OnPlayerListChanged.BindUObject(LobbyMenu->PlayerList,
		&UPlayerList::OnPlayerListUpdate);
	}

	if(!LobbyMenu->IsInViewport())
    LobbyMenu->AddToViewport();	

	SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, LobbyMenu);		
}

void ALobbyPlayerController::Client_KickPlayer_Implementation()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	GetGameInstance<UGameInstanceInfo>()->DestroySession();
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = GetWorld()->GetGameState<ASkyFlyGameStateBase>();

	UpdateLobby();
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(LobbyMenu)
	{
		LobbyMenu->RemoveFromParent();
		LobbyMenu->Destruct();
	}		
	
}

void ALobbyPlayerController::ClientUpdateLobby_Implementation()
{
	if(!LobbyMenu)
	{
		if(!LobbyMenuClass)
			return;
		
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);
		GameStateRef->OnPlayerListChanged.Unbind();
		GameStateRef->OnPlayerListChanged.BindUObject(LobbyMenu->PlayerList,
			&UPlayerList::OnPlayerListUpdate);
		
	}
	LobbyMenu->AddToViewport();

	SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, LobbyMenu);
}
