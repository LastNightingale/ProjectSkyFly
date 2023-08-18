// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftLaser.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASkyShiftLaser::ASkyShiftLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base");

	//LaserParticles->SetBeamSourcePoint(0, Owner->GetActorLocation(), 0);

	
}

// Called when the game starts or when spawned
void ASkyShiftLaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkyShiftLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//LaserParticles->SetBeamSourcePoint(0, Owner->GetActorLocation(), 0);

	UParticleSystemComponent* LaserParticle = UGameplayStatics::SpawnEmitterAttached(LaserParticleClass, Base);

	LaserParticle->SetBeamSourcePoint(0, FVector(0.f, 0.f, 0.f), 0);

}

