// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftBattery.h"

void ASkyShiftBattery::BeginPlay()
{
	Super::BeginPlay();

	BatteryBoundaries->OnComponentBeginOverlap.AddDynamic(this, &ASkyShiftBattery::OnPlayerBeginOverlap);
}

void ASkyShiftBattery::WasPicked_Implementation()
{
	RestoreValue();
	Destroy();
}

void ASkyShiftBattery::RestoreValue()
{
	//PickingPawn->Power += this->EnergyValue;
	PickingPawn->RestorePower(EnergyValue);
}

void ASkyShiftBattery::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((!PickingPawn) && Cast<ASkyFlyJetPawn>(OtherActor))
	{
		PickingPawn = Cast<ASkyFlyJetPawn>(OtherActor);
		WasPicked();
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
}
