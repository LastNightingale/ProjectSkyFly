// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
