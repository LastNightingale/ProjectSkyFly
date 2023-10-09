// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
	class ASkyFlyJetPawn* PlayerRef = nullptr;
	
	void SetHealth(float CurrentHealth, float MaxHealth);
};
