// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY()
	UBoxComponent* LaserHitBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Power")
	float LaserPower = 5000.f;

	UPROPERTY()
	USceneComponent* BaseScene;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> LaserDamage;

	FVector EndLocation;

	FTimerHandle LaserDamageHandle;

	FHitResult Hit;

	AActor* PlayerToHit;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SpawnLaser();
	void DestroyLaser();

	void SetCollisionBox(FVector LocationVector, FRotator RotationRotator, FVector ScaleVector);

	UFUNCTION()
	void OnLaserBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLaserEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamagePerTick = 5.5f;	

	UFUNCTION()
	void ApplyLaserDamage();
};
