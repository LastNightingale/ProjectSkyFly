// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/LobbyMenu.h"

#include "Kismet/KismetSystemLibrary.h"

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	//UpdateMenu();
}

void ULobbyMenu::UpdateMenu()
{
	/*if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}*/
}
