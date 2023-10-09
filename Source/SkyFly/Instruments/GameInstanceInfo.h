// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceInfo.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UGameInstanceInfo : public UGameInstance
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class UMainMenu> MainMenuClass;

	UPROPERTY()
	UMainMenu* MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class UJoinMenu> JoinMenuClass;

	UPROPERTY()
	UJoinMenu* JoinMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class UHostMenu> HostMenuClass;

	UPROPERTY()
	UHostMenu* HostMenu;

public:

	void ShowMainMenu();
	
	void ShowHostMenu();
	
	void ShowJoinMenu();
	
};
