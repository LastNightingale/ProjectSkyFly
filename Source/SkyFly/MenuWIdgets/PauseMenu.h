// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* QuitButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeButtonClick();

	UFUNCTION()
	void OnQuitButtonClick();
	
};
