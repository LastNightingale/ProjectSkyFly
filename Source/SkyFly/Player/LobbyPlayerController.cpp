// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Instruments/GameInstanceInfo.h"
#include "Instruments/SkyFlyGameStateBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerController::UpdateLobby()
{
	/*if(!IsLocalController())
	{
		return;
	}*/
	
	
	if(!IsLocalController())
	{
		//ensureMsgf(LobbyMenu != nullptr, TEXT("Not nullptr at start"));
		ClientUpdateLobby();
		return;
	}

	SessionName = GetGameInstance<UGameInstanceInfo>()->GetSessionName();
	
	if(!LobbyMenu)
	{
		//ensureMsgf(LobbyMenu != nullptr, TEXT("Not nullptr"));
		
		if(!LobbyMenuClass)
			return;

		//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);
		if(HasAuthority())
			LobbyMenu->SetServer();

		/*GameStateRef = GetWorld()->GetGameState<ASkyFlyGameStateBase>();*/

		GameStateRef->OnPlayerListChanged.Unbind();
		GameStateRef->OnPlayerListChanged.BindUObject(LobbyMenu->PlayerList,
		&UPlayerList::OnPlayerListUpdate);
	}

	//check(LobbyMenu);

	if(!LobbyMenu->IsInViewport())
    LobbyMenu->AddToViewport();	

	SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, LobbyMenu);		
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = GetWorld()->GetGameState<ASkyFlyGameStateBase>();

	check(GameStateRef);	

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

void ALobbyPlayerController::OnSessionNameChange()
{	
	GetGameInstance<UGameInstanceInfo>()->SetSessionName(SessionName);
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, SessionName);
}
