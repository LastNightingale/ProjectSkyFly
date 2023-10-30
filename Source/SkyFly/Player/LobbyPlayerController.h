// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuWIdgets/LobbyMenu.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class ULobbyMenu> LobbyMenuClass;

	UPROPERTY(ReplicatedUsing = OnSessionNameChange)
	FName SessionName;

	FTimerHandle TimerHandle;
	
public:

	UPROPERTY()
	ULobbyMenu* LobbyMenu;	

	UPROPERTY()
	class ASkyFlyGameStateBase* GameStateRef; 

	void UpdateLobby();

	UFUNCTION(Client, reliable)	
	void ClientUpdateLobby();	
	void ClientUpdateLobby_Implementation();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSessionNameChange();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
};
