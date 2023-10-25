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

	virtual void BeginPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void PostLogin (APlayerController * NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void KickPlayer(uint8 PlayerID);

	void UpdatePlayerList();

	bool ShouldSpawnAtStartSpot(AController* Player) override;

private:

	UPROPERTY()
	TArray<ASkyFlyPlayerController*> AllPlayerControllers;

	FTimerHandle TimerHandle;	
	
};

inline bool ASkyFlyGameModeBase::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}
