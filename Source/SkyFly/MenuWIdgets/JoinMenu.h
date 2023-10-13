// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "JoinMenu.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UJoinMenu : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY()
	class UGameInstanceInfo* GameInstanceRef;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* BackButton;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UEditableTextBox* ServerName;

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackButtonClick();

	UFUNCTION()
	void OnJoinButtonClick();

	UFUNCTION()
	void OnServerNameChanged(const FText& Text);
	
};
