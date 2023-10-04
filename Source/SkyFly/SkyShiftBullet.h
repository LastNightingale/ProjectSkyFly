// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SkyShiftBullet.generated.h"

UCLASS()
class SKYFLY_API ASkyShiftBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyShiftBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Moving")
		FVector Velocity = FVector(0.f);

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent* BulletMovement;

	UFUNCTION()
	void SetVelocity(FVector NewVelocity) const;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 10.f;	


};
