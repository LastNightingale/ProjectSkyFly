// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* ExitButton;
	
};
