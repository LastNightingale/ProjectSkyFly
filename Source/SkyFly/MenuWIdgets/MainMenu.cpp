// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/MainMenu.h"

#include "Instruments/GameInstanceInfo.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = GetGameInstance<UGameInstanceInfo>();
}

void UMainMenu::OnHostButtonClick()
{
	RemoveFromParent();
	GameInstance->ShowHostMenu();
}

void UMainMenu::OnJoinButtonClick()
{
	RemoveFromParent();
	GameInstance->ShowJoinMenu();
}

void UMainMenu::OnExitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
