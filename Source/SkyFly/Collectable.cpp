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
	
	BatteryBoundaries->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnPlayerBeginOverlap);
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectable::WasPicked_Implementation()
{
	RestoreValue();
	Destroy();
}

ASkyFlyJetPawn* ACollectable::GetPickingPawn()
{
	return PickingPawn;;
}

void ACollectable::RestoreValue()
{
}

void ACollectable::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((!PickingPawn) && Cast<ASkyFlyJetPawn>(OtherActor))
	{
		PickingPawn = Cast<ASkyFlyJetPawn>(OtherActor);
		WasPicked();
	}
}

