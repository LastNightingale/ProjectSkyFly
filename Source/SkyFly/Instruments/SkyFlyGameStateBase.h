// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgets/PlayerList.h"
#include "GameFramework/GameStateBase.h"
#include "SkyFlyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:

	UPROPERTY(ReplicatedUsing = OnRep_AllPlayerStates)
	TArray<APlayerState*> AllPlayerStates;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerList> PlayerListClass;
	
	void UpdatePlayerList();

	UFUNCTION()
	void OnRep_AllPlayerStates();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
	
};
