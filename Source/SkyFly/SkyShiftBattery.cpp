// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftBattery.h"

void ASkyShiftBattery::BeginPlay()
{
	Super::BeginPlay();

	
}


void ASkyShiftBattery::RestoreValue()
{
	PickingPawn->RestorePower(EnergyValue);
}

