// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "SkyShiftLaser.generated.h"


UCLASS()
class SKYFLY_API ASkyShiftLaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyShiftLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* LaserParticleClass;

	UPROPERTY(EditAnywhere, Category = "Particles")
	class UParticleSystem* LaserHitParticleClass;
		
	UStaticMeshComponent* Base;

	UPROPERTY(Replicated)
	UParticleSystemComponent* Laser = nullptr;

	UPROPERTY(Replicated)
	UParticleSystemComponent* LaserHit = nullptr;

	UPROPERTY(EditAnywhere, Category = "Power")
	float LaserPower = 5000.f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SpawnLaser();
	void DestroyLaser();
};
