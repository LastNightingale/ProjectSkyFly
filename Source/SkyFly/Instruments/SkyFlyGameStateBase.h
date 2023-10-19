// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgets/PlayerList.h"
#include "GameFramework/GameStateBase.h"
#include "SkyFlyGameStateBase.generated.h"

DECLARE_DELEGATE_OneParam(FPlayerListChanged, TArray<APlayerState*>)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerListChangedClient, TArray<APlayerState*>)

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

	FPlayerListChanged OnPlayerListChanged;

	FPlayerListChangedClient OnPlayerListChangedClient;
	
};
