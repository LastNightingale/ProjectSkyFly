// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

void ASkyFlyHUD::BeginPlay()
{
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), UISwitcherClass);
	Widget->AddToViewport();
	UISwitcher = Cast<UWidgetSwitcher>(Widget->WidgetTree->FindWidget(FName("Switcher")));
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	UISwitcher->SetActiveWidgetIndex(0);

	/*CanvasPanelWidget = CreateWidget<UUserWidget>(GetOwningPlayerController());

	CanvasPanel = CanvasPanelWidget->WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());*/

	//CanvasPanelWidget->AddToViewport();
	
	//Canvas = Cast<UCanvasPanel>(CanvasPanel);

}

void ASkyFlyHUD::SetUI(uint8 index)
{
	if (!UIWidgets[index])
		return;

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), UIWidgets[index]);

	if (!NewWidget)
		return;

	NewWidget->AddToViewport();
	APlayerController* JetPlayer = GetOwningPlayerController();
	JetPlayer->SetShowMouseCursor(true);
	UWidgetBlueprintLibrary::SetInputMode_UIOnly(JetPlayer, NewWidget);
}
