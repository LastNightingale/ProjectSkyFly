// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkyShiftBullet.h"
#include "SkyShiftLaser.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SkyFlyJetPawn.generated.h"

//class ASkyShiftLaser;

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

	UPROPERTY(EditAnywhere, Category = "Components")
	class UFloatingPawnMovement* MovingComponent;

	UPROPERTY(Replicated, EditAnywhere, Category = "Movement")
	float Thrust = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxThrust = 5000.f;	

	UPROPERTY(Replicated, EditAnywhere, Category = "Shooting")
	bool bInPowerMode = false;

	UPROPERTY(Replicated, EditAnywhere, Category = "Shooting")
	bool bOnLaserFire = false;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	float LaserPower = 5000.f;

	UPROPERTY(Replicated, EditAnywhere, Category = "Shooting")
	uint8 Ammo = 10;

	UPROPERTY(Replicated, EditAnywhere, Category = "Shooting")
	float Power = 100.f;

	UPROPERTY(Replicated, EditAnywhere, Category = "Shooting")
	float MaxPower = 100.f;

	FTimerHandle LaserTimerHandle;

	UPROPERTY(Replicated, EditAnywhere, Category = "Movement")
	FVector ForvardVelocity = FVector(0.f);	

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<ASkyShiftBullet> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<ASkyShiftLaser> LaserClass;

	UPROPERTY(EditAnywhere, Category = "Laser")
	class UParticleSystem* LaserParticleClass;

	UPROPERTY(EditAnywhere, Category = "Laser")
	class UParticleSystem* LaserHitParticleClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WidgetBP[2];

	UPROPERTY(Replicated)
	UParticleSystemComponent* Laser = nullptr;

	UPROPERTY(Replicated)
	bool bIsLaserSpawned = false;

	UPROPERTY(Replicated)
	UParticleSystemComponent* LaserHit = nullptr;

	UPROPERTY(Replicated)
	bool bIsLaserHitSpawned = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	

	UFUNCTION()
	void JetThrust(float value);

	UFUNCTION()
	void MoveUp(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void Roll(float value);

	UFUNCTION()
	void OnBulletFire();

	UFUNCTION()
	void ChangeMode();

	UFUNCTION()
	void SpawnLaser();

	UFUNCTION()
	void DestroyLaser();

	UFUNCTION(Server, unreliable, WithValidation)
	void Server_OnBulletFire(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);
	bool Server_OnBulletFire_Validate(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);
	void Server_OnBulletFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

	UFUNCTION(Server, unreliable, WithValidation)
	void Server_SetRotation(FVector Direction, float value);
	bool Server_SetRotation_Validate(FVector Direction, float value);
	void Server_SetRotation_Implementation(FVector Direction, float value);

	UFUNCTION(Server, unreliable, WithValidation)
	void Server_SetLinearVelocity(FVector NewVelocity);
	bool Server_SetLinearVelocity_Validate(FVector NewVelocity);
	void Server_SetLinearVelocity_Implementation(FVector NewVelocity);
	
	UFUNCTION(Server, unreliable, WithValidation)
	void Server_OnLaserFire(FVector SpawnLocation, FRotator SpawnRotation);
	bool Server_OnLaserFire_Validate(FVector SpawnLocation, FRotator SpawnRotation);
	void Server_OnLaserFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION()
	void PowerWithdraw();

	UFUNCTION()
	void BulletWithdraw();

	UFUNCTION(BlueprintPure, Category = "Counter")
	float GetPower();

	UFUNCTION(BlueprintPure, Category = "Counter")
	float GetMaxPower();

	UFUNCTION(BlueprintPure, Category = "Counter")
	uint8 GetAmmo();

	UFUNCTION(BlueprintPure, Category = "Counter")
	bool GetPowerMode();
};


enum UIMode
{
	UI_PowerOff = 0,
	UI_PowerOn = 1
};
