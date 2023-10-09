// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "SkyShiftBattery.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyShiftBattery : public ACollectable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	float EnergyValue = 15.f;		

	void RestoreValue() override;
	
};
