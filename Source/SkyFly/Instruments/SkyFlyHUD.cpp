// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyHUD.h"

#include "SkyFlyGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"

void ASkyFlyHUD::BeginPlay()
{
	Super::BeginPlay();
	//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	
	/*SwitcherWidget = CreateWidget<UPowerModeSwitcher>(GetWorld(), UISwitcherClass);
	PlayerList = CreateWidget<UPlayerList>(GetWorld(), PlayerListClass);

	GetWorld()->GetGameState<ASkyFlyGameStateBase>()->OnPlayerListChanged.Unbind();
	GetWorld()->GetGameState<ASkyFlyGameStateBase>()->OnPlayerListChanged.BindUObject(PlayerList,
		&UPlayerList::OnPlayerListUpdate);
	
	SwitcherWidget->AddToViewport();	
	
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	SwitcherWidget->UISwitcher->SetActiveWidgetIndex(0);*/
}

void ASkyFlyHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
}

void ASkyFlyHUD::SetupHUD()
{
	SwitcherWidget = CreateWidget<UPowerModeSwitcher>(GetWorld(), UISwitcherClass);
	PlayerList = CreateWidget<UPlayerList>(GetWorld(), PlayerListClass);

	GetWorld()->GetGameState<ASkyFlyGameStateBase>()->OnPlayerListChanged.Unbind();
	GetWorld()->GetGameState<ASkyFlyGameStateBase>()->OnPlayerListChanged.BindUObject(PlayerList,
		&UPlayerList::OnPlayerListUpdate);
	
	SwitcherWidget->AddToViewport();	
	
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	SwitcherWidget->UISwitcher->SetActiveWidgetIndex(0);
}

void ASkyFlyHUD::OpenPlayerList()
{
	if(!PlayerList->IsInViewport())
	{
		PlayerList->AddToViewport();
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayerController(), PlayerList);
	}		
}

void ASkyFlyHUD::ClosePlayerList()
{
	if(PlayerList->IsInViewport())
	{
		PlayerList->RemoveFromParent();
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	}	
}

void ASkyFlyHUD::EndHUD()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Ended Game")));*/

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

void ASkyFlyHUD::SetUI(uint8 Index)
{
	if (!UIWidgets[Index])
		return;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), UIWidgets[Index]);

	if (!NewWidget)
		return;

	

	NewWidget->AddToViewport();
	APlayerController* JetPlayer = GetOwningPlayerController();
	JetPlayer->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnly(JetPlayer, NewWidget);
}
