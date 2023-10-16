// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuWIdgets/LobbyMenu.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	UPROPERTY()
	ULobbyMenu* LobbyMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class ULobbyMenu> LobbyMenuClass;
	
public:

	void UpdateLobby();	

	UFUNCTION(Client, reliable)	
	void ClientUpdateLobby();	
	void ClientUpdateLobby_Implementation();
	
	
};