// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftLaser.h"

// Sets default values
ASkyShiftLaser::ASkyShiftLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserParticles = CreateDefaultSubobject<UParticleSystemComponent>("Laser Particles");

	
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

}

