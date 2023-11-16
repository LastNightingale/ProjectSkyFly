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

	virtual void GenericPlayerInitialization(AController* C) override;

	virtual void Logout(AController* Exiting) override;

	void KickPlayer(uint8 PlayerID);

	void UpdatePlayerList();

	UPROPERTY()
	TArray<class ALobbyPlayerController*> AllPlayerControllers;

	void ResetTimer();
	
};
