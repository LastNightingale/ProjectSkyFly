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

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class UMainMenu> MainMenuClass;

	UPROPERTY()
	UMainMenu* MainMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class UJoinMenu> JoinMenuClass;

	UPROPERTY()
	UJoinMenu* JoinMenu;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<class UHostMenu> HostMenuClass;

	UPROPERTY()
	UHostMenu* HostMenu;	

	uint8 NumberOfPlayers;
	
	FName NameOfServer;

	FName NameOfServerToJoin;

	bool IsLan;

public:	

	/*UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	//UFUNCTION(BluepringPure)
	void ShowHostMenu();

	//UFUNCTION(BluepringPure)
	void ShowJoinMenu();	*/

	void LaunchLobby(uint8 PlayerNumber, bool LAN, const FName& NameServer);

	void JoinLobby(const FName& NameServer);

	void SetSessionName(const FName Name);

	FName GetSessionName() const;

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

	void SetJoinable(bool bIsJoinable);

	bool CheckConnectionAmount();
};
