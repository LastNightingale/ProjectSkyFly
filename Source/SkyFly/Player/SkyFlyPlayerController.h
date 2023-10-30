// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIWidgets/PlayerList.h"
#include "UIWidgets/PowerModeSwitcher.h"
#include "SkyFlyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	void Respawn();

	AActor* FindPlayerStarts();

	UFUNCTION(Client, Reliable)
	void Client_KickPlayer();
	void Client_KickPlayer_Implementation();

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
