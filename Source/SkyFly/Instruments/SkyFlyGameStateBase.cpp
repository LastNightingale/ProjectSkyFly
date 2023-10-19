// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyGameStateBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

void ASkyFlyGameStateBase::UpdatePlayerList()
{	
	AllPlayerStates = PlayerArray;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Was updated: %d"), OnPlayerListChanged.ExecuteIfBound(PlayerArray)));
	

	/*TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(),FoundWidgets, PlayerListClass, false);	

	for(auto FoundWidget : FoundWidgets)
	{		
		Cast<UPlayerList>(FoundWidget)->UpdateWidget();
	}	*/
}

void ASkyFlyGameStateBase::OnRep_AllPlayerStates()
{
	/*TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(),FoundWidgets, PlayerListClass, false);*/

	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("%d = Num"), FoundWidgets.Num()));*/

	/*if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
	FString::Printf(TEXT("Server Func Worked")));
	}*/

	OnPlayerListChanged.ExecuteIfBound(AllPlayerStates);

	

	/*for(auto FoundWidget : FoundWidgets)
	{		
		Cast<UPlayerList>(FoundWidget)->UpdateWidget();
	}*/
}

void ASkyFlyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyGameStateBase, AllPlayerStates);
}
