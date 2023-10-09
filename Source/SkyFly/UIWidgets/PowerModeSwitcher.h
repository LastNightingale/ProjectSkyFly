// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "PowerModeSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UPowerModeSwitcher : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* UISwitcher;
	
};
