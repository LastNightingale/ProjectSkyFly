// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SkyFlyGameState.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyGameState : public AGameState
{
	GENERATED_BODY()
public:

	UPROPERTY(ReplicatedUsing = OnRep_AllPlayerStates)
	TArray<APlayerState*> AllPlayerStates;
	
	void UpdatePlayerList();

	UFUNCTION()
	void OnRep_AllPlayerStates();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
	
};
