// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Instruments/GameInstanceInfo.h"
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
		
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);
	}

	if(!LobbyMenu->IsInViewport())
    LobbyMenu->AddToViewport();

	

	SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, LobbyMenu);	

	
}

void ALobbyPlayerController::ClientUpdateLobby_Implementation()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Menu: %d"), LobbyMenu != nullptr));*/
	if(!LobbyMenu)
	{
		if(!LobbyMenuClass)
			return;
		
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyMenuClass);
	}

	//if(!LobbyMenu->IsInViewport())
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
