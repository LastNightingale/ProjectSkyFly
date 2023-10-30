// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	
	FTimerHandle TimerHandle;
	

public:

	void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void UpdatePlayerList();
	
};
