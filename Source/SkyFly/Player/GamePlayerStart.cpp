// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GamePlayerStart.h"

bool AGamePlayerStart::IsTaken() const
{
	return bIsTaken;
}

void AGamePlayerStart::Take()
{
	bIsTaken = true;
}
