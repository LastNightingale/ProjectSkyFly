// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "SkyShiftAmmo.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API ASkyShiftAmmo : public ACollectable
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	uint8 AmmoValue = 1;

	void WasPicked_Implementation() override;

	void RestoreValue() override;

	UFUNCTION()
		void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
