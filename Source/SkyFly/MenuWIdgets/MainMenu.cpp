// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/MainMenu.h"

#include "Instruments/GameInstanceInfo.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstanceRef = GetGameInstance<UGameInstanceInfo>();

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonClick);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinButtonClick);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnExitButtonClick);
}

void UMainMenu::OnHostButtonClick()
{
	if(IsInViewport())
		RemoveFromParent();
	GameInstanceRef->ShowHostMenu();	
}

void UMainMenu::OnJoinButtonClick()
{
	if(IsInViewport())
		RemoveFromParent();
	GameInstanceRef->ShowJoinMenu();
}

void UMainMenu::OnExitButtonClick()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
