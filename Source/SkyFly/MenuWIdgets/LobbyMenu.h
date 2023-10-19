// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "UIWidgets/PlayerList.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()


private:
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* LeaveButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* PlayerListBox;	

	UPROPERTY(EditDefaultsOnly, Category = "PlayerList")
	TSubclassOf<class UPlayerList> PlayerListClass;
	
public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UPlayerList* PlayerList;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnLeaveButtonClick();

	UFUNCTION()
	void OnStartButtonClick();

	void UpdateMenu();

	void SetServer();
	
};


