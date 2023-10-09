// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftAmmo.h"

void ASkyShiftAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}



void ASkyShiftAmmo::RestoreValue()
{
	PickingPawn->RestoreAmmo(AmmoValue);
}


