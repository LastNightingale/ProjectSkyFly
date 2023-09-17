// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftBatteries.h"

// Sets default values
ASkyShiftBatteries::ASkyShiftBatteries()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BatteryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Battery Mesh"));
	BatteryBoundaries = CreateDefaultSubobject<USphereComponent>(TEXT("Battery Boundaries"));

	SetRootComponent(BatteryMesh);

	BatteryBoundaries->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//BatteryBoundaries->SetWorldLocation(BatteryMesh->GetComponentLocation());
	BatteryBoundaries->SetRelativeLocation({ 0.f, 0.f, 0.f });
}

// Called when the game starts or when spawned
void ASkyShiftBatteries::BeginPlay()
{
	Super::BeginPlay();
	
	BatteryBoundaries->OnComponentBeginOverlap.AddDynamic(this, &ASkyShiftBatteries::OnPlayerBeginOverlap);
}

// Called every frame
void ASkyShiftBatteries::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkyShiftBatteries::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((!PickingPawn) && Cast<APawn>(OtherActor))
	{
		PickingPawn = Cast<APawn>(OtherActor);
		WasPicked();
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
}

void ASkyShiftBatteries::WasPicked_Implementation()
{
	Destroy();
}

APawn* ASkyShiftBatteries::GetPickingPawn()
{
	return PickingPawn;
}


