// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "HostMenu.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UHostMenu : public UUserWidget
{
	GENERATED_BODY()

private:	

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* DecreasePlayersButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* IncreasePlayersButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* DecreaseNetButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* IncreaseNetButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* CreateButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UEditableTextBox* ServerName;

	UPROPERTY()
	class UGameInstanceInfo* GameInstanceRef;
	
	uint8 MaxNumberOfPlayers;

	bool EnabledLAN;	

public:

	UPROPERTY(BlueprintReadOnly)
	uint8 NumberOfPlayers;
	
	UPROPERTY(BlueprintReadOnly)
	FText NetText;

	virtual void NativeConstruct() override;	

	UFUNCTION()
	void OnBackButtonClick();

	UFUNCTION()
	void OnCreateButtonClick();

	UFUNCTION()
	void OnDecreasePlayersButtonClick();

	UFUNCTION()
	void OnIncreasePlayersButtonClick();

	UFUNCTION()
	void OnDecreaseNetButtonClick();

	UFUNCTION()
	void OnIncreaseNetButtonClick();

	UFUNCTION()
	void OnServerNameChanged(const FText& Text);
	
	
};
