// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "SkyFlyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> UISwitcherClass = nullptr;

	UWidgetSwitcher* UISwitcher = nullptr;

	uint8 CurrentUI = 0;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> UIWidgets[1];

	/*UUserWidget* CanvasPanelWidget;

	UCanvasPanel* CanvasPanel;*/

	void SetUI(uint8 index);
};
