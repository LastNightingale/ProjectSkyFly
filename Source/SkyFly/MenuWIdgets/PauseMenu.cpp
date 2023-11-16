// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWIdgets/PauseMenu.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Instruments/GameInstanceInfo.h"
#include "Instruments/SkyFlyGameModeBase.h"
#include "Instruments/SkyFlyGameStateBase.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumeButtonClick);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::OnQuitButtonClick);
}

void UPauseMenu::OnResumeButtonClick()
{
	this->RemoveFromParent();
	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
	PC->bShowMouseCursor = true;
}

void UPauseMenu::OnQuitButtonClick()
{
	if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		for(int Iter = GetWorld()->GetGameState<ASkyFlyGameStateBase>()->AllPlayerStates.Num() - 1; Iter > 0; Iter--)
		{
			GetWorld()->GetAuthGameMode<ASkyFlyGameModeBase>()->KickPlayer(Iter);
		}			
	}
	GetGameInstance<UGameInstanceInfo>()->DestroySession();
}
