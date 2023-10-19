// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig)
class SKYFLY_API UProjectDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
 

public:

	UPROPERTY(Config, EditDefaultsOnly, Category = "PlayerUI")
	TArray<FColor> PlayerColors;
	
};
