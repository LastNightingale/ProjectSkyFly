// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyShiftBullet.h"


// Sets default values
ASkyShiftBullet::ASkyShiftBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("Bullet Mesh");

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Bullet Movement");
	
	BulletMovement->InitialSpeed = 5000.f;
	BulletMovement->MaxSpeed = 10000.f;

	SetRootComponent(BulletMesh);
}

// Called when the game starts or when spawned
void ASkyShiftBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkyShiftBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkyShiftBullet::SetVelocity(FVector NewVelocity)
{
	BulletMovement->Velocity = NewVelocity;
}



