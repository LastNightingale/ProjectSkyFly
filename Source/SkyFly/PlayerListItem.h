// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerListItem.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UPlayerListItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UButton* KickButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* UsernameText;

	FText Text;

	uint8 PlayerID;

	UFUNCTION()
	void OnClick();
};
