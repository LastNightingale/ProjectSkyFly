// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"

// Sets default values
ACollectable::ACollectable()
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
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectable::WasPicked_Implementation()
{
	
}

ASkyFlyJetPawn* ACollectable::GetPickingPawn()
{
	return PickingPawn;;
}

void ACollectable::RestoreValue()
{
}

