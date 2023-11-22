// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameInstanceInfo.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UGameInstanceInfo : public UGameInstance
{
	GENERATED_BODY()

private:

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	uint8 NumberOfPlayers;
	
	FName NameOfServer;

	FName NameOfServerToJoin;

	bool IsLan;

public:	

	void LaunchLobby(uint8 PlayerNumber);

	void JoinLobby(const FName& NameServer);

	virtual void Init() override;

	void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	
	void OnFindSessionComplete(bool Succeeded);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnDestroySessionComplete(FName ServerName, bool Succeeded);

	void CreateSession();

	void DestroySession();

	void JoinSession();

	void StartMatch();

	void ReturnToLobby();
};
