// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyCanvas.h"
#include "Components/ProgressBar.h"

void USkyFlyCanvas::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
