// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkyFlyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void Respawn();

	AActor* FindPlayerStarts();

	UFUNCTION(Client, Reliable)
	void Client_KickPlayer();
	void Client_KickPlayer_Implementation();
	
};
