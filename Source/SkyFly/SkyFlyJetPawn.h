// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkyShiftBullet.h"
#include "SkyShiftLaser.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Containers/EnumAsByte.h"
#include "Blueprint/UserWidget.h"
#include "SkyFlyHUD.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SkyFlyJetPawn.generated.h"

//class ASkyShiftLaser;

enum UIMode
{
	/*UI_PowerOff = 0,
	UI_PowerOn = 1,*/
	UI_PauseMenu = 0
};


UENUM()
enum LaserState
{
	FireOff = 0 UMETA(DisplayName = "FireOff"),
	FireOn = 1 UMETA(DisplayName = "FireOn")
};

UENUM()
enum PowerMode
{
	Off = 0 UMETA(DisplayName = "Off"),
	On = 1 UMETA(DisplayName = "On")
};


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

	UPROPERTY(Replicated, EditAnywhere, Category = "Movement")
	float Thrust = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxThrust = 5000.f;	

	UPROPERTY(Replicated, EditAnywhere, Category = "Health")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Shooting")
	TEnumAsByte<PowerMode> CurrentPowerMode = PowerMode::Off;

	UPROPERTY(ReplicatedUsing = HandleLaser, EditDefaultsOnly, Category = "Shooting")
	TEnumAsByte <LaserState> CurrentLaserState = LaserState::FireOff;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float LaserPower = 5000.f;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Shooting")
	uint8 Ammo = 10;

	UPROPERTY(ReplicatedUsing = HandleLaser, EditDefaultsOnly, Category = "Shooting")
	float Power = 100.f;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Shooting")
	float MaxPower = 100.f;

	FTimerHandle LaserTimerHandle;	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Movement")
	FVector ForvardVelocity = FVector(0.f);	

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ASkyShiftBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ASkyShiftLaser> LaserClass;	

	/*UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WidgetBP[3];*/
		
	UPROPERTY(Replicated)
	ASkyShiftLaser* Laser = nullptr;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	ASkyFlyHUD* PlayerHUD = nullptr;

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

	UFUNCTION()
	void OnPause();

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
	void Server_OnLaserFire();
	bool Server_OnLaserFire_Validate();
	void Server_OnLaserFire_Implementation();

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
	TEnumAsByte<PowerMode> GetPowerMode();

	UFUNCTION()
	void HandleLaser();

	UFUNCTION(BlueprintPure, Category = "Counter")
	float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Counter")
	float GetMaxHealth();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnKillZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};




