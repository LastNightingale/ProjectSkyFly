// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"

void ASkyFlyHUD::BeginPlay()
{
	SwitcherWidget = CreateWidget<UPowerModeSwitcher>(GetWorld(), UISwitcherClass);
	SwitcherWidget->AddToViewport();
	
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	SwitcherWidget->UISwitcher->SetActiveWidgetIndex(0);
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
