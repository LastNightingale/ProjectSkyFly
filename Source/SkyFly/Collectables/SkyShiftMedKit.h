// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "SkyShiftMedKit.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyShiftMedKit : public ACollectable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	float HealthValue = 10.f;	

	void RestoreValue() override;	
	
};
