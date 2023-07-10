// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SkyFlyJetPawn.generated.h"

UCLASS()
class SKYFLY_API ASkyFlyJetPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkyFlyJetPawn();

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* JetMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Thrust = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxThrust = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravityThreshHold = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector ForvardVelocity = FVector(0.f);

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector UpVelocity = FVector(0.f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void JetThrust(float value);


	UFUNCTION()
	void MoveUp(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void Roll(float value);
};
