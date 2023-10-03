// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHPWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
