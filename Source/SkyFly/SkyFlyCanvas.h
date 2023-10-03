// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkyFlyCanvas.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API USkyFlyCanvas : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* Canvas;


	void SetHealth(float CurrentHealth, float MaxHealth);
	
};
