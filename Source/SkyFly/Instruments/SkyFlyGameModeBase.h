// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectDeveloperSettings.h"
#include "Player/SkyFlyPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "SkyFlyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void GenericPlayerInitialization(AController* C) override;

	virtual void BeginPlay() override;

	virtual void PostLogin (APlayerController * NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void KickPlayer(uint8 PlayerID);

	void UpdatePlayerList();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:

	UPROPERTY()
	TArray<ASkyFlyPlayerController*> AllPlayerControllers;

	FTimerHandle TimerHandle;	
	
};
