// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyPlayerController.h"

#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Instruments/SkyFlyGameModeBase.h"
#include "Instruments/SkyFlyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GamePlayerStart.h"
#include "Instruments/GameInstanceInfo.h"
#include "Instruments/SkyFlyGameStateBase.h"

class UGameInstanceInfo;
class AGamePlayerStart;

void ASkyFlyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*if(IsLocalController())
		SetupHUD();*/
}

void ASkyFlyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	
	Super::EndPlay(EndPlayReason);

	//GetHUD<ASkyFlyHUD>()->EndHUD();
}

void ASkyFlyPlayerController::Respawn()
{
	if(APawn* OldPlayerPawn = GetPawn())
		OldPlayerPawn->Destroy();
		
	
	APawn* PlayerPawn = GetWorld()->GetAuthGameMode<ASkyFlyGameModeBase>()->SpawnDefaultPawnFor(this,
		FindPlayerStarts());	

	this->Possess(PlayerPawn);
}

AActor* ASkyFlyPlayerController::FindPlayerStarts()
{
	for(TActorIterator<AGamePlayerStart> Iter(GetWorld()); Iter; ++Iter)
	{
		if(!Iter->IsTaken())
		{
			Iter->Take();
			return *Iter;
		}
	}	
	return nullptr;
}

void ASkyFlyPlayerController::Client_KickPlayer_Implementation()
{
	GetGameInstance<UGameInstanceInfo>()->DestroySession();
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0),
			EQuitPreference::Quit, false);
	//UGameplayStatics::OpenLevel(GetWorld(), "Minimal_Default");
}

void ASkyFlyPlayerController::SetupHUD()
{
	SwitcherWidget = CreateWidget<UPowerModeSwitcher>(GetWorld(), UISwitcherClass);
	PlayerList = CreateWidget<UPlayerList>(GetWorld(), PlayerListClass);

	GetWorld()->GetGameState<ASkyFlyGameStateBase>()->OnPlayerListChanged.Unbind();
	GetWorld()->GetGameState<ASkyFlyGameStateBase>()->OnPlayerListChanged.BindUObject(PlayerList,
		&UPlayerList::OnPlayerListUpdate);
	
	SwitcherWidget->AddToViewport();	
	
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
	SwitcherWidget->UISwitcher->SetActiveWidgetIndex(0);
}

void ASkyFlyPlayerController::OpenPlayerList()
{
	if(!PlayerList->IsInViewport())
	{
		PlayerList->AddToViewport();
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, PlayerList);
	}	
}

void ASkyFlyPlayerController::EndHUD()
{

	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("END HUD")));*/
	
	if(SwitcherWidget)
	{		
		SwitcherWidget->RemoveFromParent();
		SwitcherWidget->Destruct();
	}
	if(PlayerList)
	{
		PlayerList->RemoveFromParent();
		PlayerList->Destruct();		
	}
}

void ASkyFlyPlayerController::SetUI(uint8 Index)
{
	if (!UIWidgets[Index])
		return;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), UIWidgets[Index]);

	if (!NewWidget)
		return;	

	NewWidget->AddToViewport();
	this->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, NewWidget);
}
