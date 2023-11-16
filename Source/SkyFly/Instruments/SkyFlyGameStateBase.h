// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIWidgets/PlayerList.h"
#include "GameFramework/GameStateBase.h"
#include "SkyFlyGameStateBase.generated.h"

DECLARE_DELEGATE_OneParam(FPlayerListChanged, TArray<APlayerState*>)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerListChangedCast, TArray<APlayerState*>, Players);

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyFlyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_AllPlayerStates)
	TArray<APlayerState*> AllPlayerStates;

	UPROPERTY(Replicated)
	TArray<FString> AllPlayerNames;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerList> PlayerListClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerList> PlayerListGameClass;
	
	void UpdatePlayerList();

	UFUNCTION()
	void OnRep_AllPlayerStates();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

	FPlayerListChanged OnPlayerListChanged;

	void UpdateControllerWidget(APlayerController* NewPlayer);

	UPROPERTY()
	const UProjectDeveloperSettings* ProjectSettings;

	void CheckStateOfPlayers();

	void EndGame();
};
