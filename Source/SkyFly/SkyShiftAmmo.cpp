// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftAmmo.h"

void ASkyShiftAmmo::BeginPlay()
{
	Super::BeginPlay();

	BatteryBoundaries->OnComponentBeginOverlap.AddDynamic(this, &ASkyShiftAmmo::OnPlayerBeginOverlap);
}

void ASkyShiftAmmo::WasPicked_Implementation()
{
	RestoreValue();
	Destroy();
}

void ASkyShiftAmmo::RestoreValue()
{
	PickingPawn->RestoreAmmo(AmmoValue);
}

void ASkyShiftAmmo::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((!PickingPawn) && Cast<ASkyFlyJetPawn>(OtherActor))
	{
		PickingPawn = Cast<ASkyFlyJetPawn>(OtherActor);
		WasPicked();
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
}
