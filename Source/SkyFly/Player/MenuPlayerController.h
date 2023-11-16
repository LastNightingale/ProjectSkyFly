// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();
	
	void ShowHostMenu();
	
	void ShowJoinMenu();

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
	
};
