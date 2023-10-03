// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftMedKit.h"

void ASkyShiftMedKit::BeginPlay()
{
	Super::BeginPlay();

	BatteryBoundaries->OnComponentBeginOverlap.AddDynamic(this, &ASkyShiftMedKit::OnPlayerBeginOverlap);
}

void ASkyShiftMedKit::WasPicked_Implementation()
{
	RestoreValue();
	Destroy();
}

void ASkyShiftMedKit::RestoreValue()
{
	PickingPawn->RestoreHealth(HealthValue);
}

void ASkyShiftMedKit::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((!PickingPawn) && Cast<ASkyFlyJetPawn>(OtherActor))
	{
		PickingPawn = Cast<ASkyFlyJetPawn>(OtherActor);
		WasPicked();
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
}
