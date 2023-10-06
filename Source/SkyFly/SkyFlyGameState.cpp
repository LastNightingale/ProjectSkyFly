// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyGameState.h"

#include "Net/UnrealNetwork.h"

void ASkyFlyGameState::UpdatePlayerList()
{
	AllPlayerStates = PlayerArray;
}

void ASkyFlyGameState::OnRep_AllPlayerStates()
{
	
}

void ASkyFlyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyGameState, AllPlayerStates);
}
