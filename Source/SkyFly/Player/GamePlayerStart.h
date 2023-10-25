// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GamePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API AGamePlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

	bool IsTaken() const;

	void Take();

private:

	bool bIsTaken = false;
	
};
