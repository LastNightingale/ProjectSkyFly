// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgets/PlayerListItem.h"
#include "Blueprint/UserWidget.h"
#include "PlayerList.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UPlayerList : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UScrollBox* PlayerListScrollBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerListItem> ItemClass;

	void UpdateWidget();

	virtual void NativeConstruct() override;

	UPROPERTY()
	const class UProjectDeveloperSettings* ProjectSettings;

	UFUNCTION()
	void OnPlayerListUpdate(TArray<APlayerState*> Players);
	
	/*UFUNCTION(Client, reliable)	
	void ClientUpdateLobby();	
	void ClientUpdateLobby_Implementation();*/
};
