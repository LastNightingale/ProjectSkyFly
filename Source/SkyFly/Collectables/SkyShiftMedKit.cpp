// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftMedKit.h"

void ASkyShiftMedKit::BeginPlay()
{
	Super::BeginPlay();	
}



void ASkyShiftMedKit::RestoreValue()
{
	PickingPawn->RestoreHealth(HealthValue);
}


