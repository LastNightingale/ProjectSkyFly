// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgets/PlayerList.h"
#include "GameFramework/HUD.h"
#include "UIWidgets/PowerModeSwitcher.h"
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> UISwitcherClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerList> PlayerListClass;

	UPROPERTY()
	UPowerModeSwitcher* SwitcherWidget;

	UPROPERTY()
	UPlayerList* PlayerList;

	uint8 CurrentUI = 0;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> UIWidgets[1];

	void SetupHUD();

	void OpenPlayerList();
	
	void ClosePlayerList();

	void EndHUD();

	void SetUI(uint8 Index);
};
