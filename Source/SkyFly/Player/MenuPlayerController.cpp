// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MenuPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MenuWIdgets/HostMenu.h"
#include "MenuWIdgets/JoinMenu.h"
#include "MenuWIdgets/MainMenu.h"

void AMenuPlayerController::ShowMainMenu()
{
	if(!MainMenuClass)
		return;
	
	if(!MainMenu)
	{		
		MainMenu = CreateWidget<UMainMenu>(this,
			MainMenuClass);
	}

	if(!MainMenu->IsInViewport())
		MainMenu->AddToViewport();

	this->SetShowMouseCursor(true);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, MainMenu);
}

void AMenuPlayerController::ShowHostMenu()
{
	if(!HostMenuClass)
		return;
	
	if(!HostMenu)
	{		
		HostMenu = CreateWidget<UHostMenu>(this,	HostMenuClass);
	}

	if(!HostMenu->IsInViewport())
		HostMenu->AddToViewport();
}

void AMenuPlayerController::ShowJoinMenu()
{
	if(!JoinMenuClass)
		return;
		
	CreateWidget<UJoinMenu>(this, JoinMenuClass)->AddToViewport();
}
