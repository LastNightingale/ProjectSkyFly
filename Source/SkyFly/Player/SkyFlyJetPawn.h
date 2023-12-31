// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/SkyShiftBullet.h"
#include "Weapons/SkyShiftLaser.h"
#include "GameFramework/Pawn.h"
#include "UIWidgets/EnemyHPWidget.h"
#include "Components/SphereComponent.h"
#include "Containers/EnumAsByte.h"
#include "Blueprint/UserWidget.h"
#include "Instruments/SkyFlyHUD.h"
#include "SkyFlyJetPawn.generated.h"

enum EUIMode
{
	/*UI_PowerOff = 0,
	UI_PowerOn = 1,*/
	UI_PauseMenu = 0
};


UENUM()
enum ELaserState
{
	FireOff = 0,
	FireOn = 1 
  };

UENUM()
enum EPowerMode
{
	Off = 0,
	On = 1 
};


UCLASS()
class SKYFLY_API ASkyFlyJetPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkyFlyJetPawn();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* JetMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* Camera;	

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* SpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UWidgetComponent* HealthBarWidget;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Movement")
	float Thrust = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxThrust = 2500.f;	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Health")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Shooting")
	TEnumAsByte<EPowerMode> CurrentPowerMode = EPowerMode::Off;

	UPROPERTY(ReplicatedUsing = HandleLaser, EditDefaultsOnly, Category = "Shooting")
	TEnumAsByte <ELaserState> CurrentLaserState = ELaserState::FireOff;

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
	FVector ForwardVelocity = FVector(0.f);	

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ASkyShiftBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ASkyShiftLaser> LaserClass;	

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EnemyHPWidgetClass;	
		
	UPROPERTY(Replicated)
	ASkyShiftLaser* Laser;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	UPROPERTY()
	ASkyFlyHUD* PlayerHUD;

	UPROPERTY()
	class ASkyFlyPlayerController* PlayerController;	

	UPROPERTY(Replicated)
	UEnemyHPWidget* PlayerHPWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Plane")
	float TurnSpeed;

	/** Current yaw speed */
	UPROPERTY(Replicated)
	float CurrentYawSpeed;

	/** Current pitch speed */
	UPROPERTY(Replicated)
	float CurrentPitchSpeed;

	/** Current roll speed */
	UPROPERTY(Replicated)
	float CurrentRollSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	bool bIsGhosted;

	UPROPERTY(ReplicatedUsing = SetHPColor)
	FLinearColor HPColor;

	UFUNCTION()
	void SetHPColor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, 
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION()
	void JetThrust(float value);

	UFUNCTION()
	void MoveUp(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void Roll(float Value);

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

	UFUNCTION()
	void OpenPlayerPanel();

	UFUNCTION()
	void ClosePlayerPanel();

	void SetGhostedPawn();

	void RestorePower(float Value);

	void RestoreHealth(float Value);

	void RestoreAmmo(uint8 Value);

	void SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

	UFUNCTION(Server, Unreliable)
	void Server_OnBulletFire(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);
	void Server_OnBulletFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

	UFUNCTION(Server, unreliable)
	void Server_SetRotation(FVector Direction, float value);
	void Server_SetRotation_Implementation(FVector Direction, float value);

	UFUNCTION(Server, unreliable)
	void Server_MoveUp(float Value);
	void Server_MoveUp_Implementation(float Value);

	UFUNCTION(Server, unreliable)
	void Server_MoveRight(float Value);
	void Server_MoveRight_Implementation(float Value);

	UFUNCTION(Server, unreliable)
	void Server_Roll(float Value);
	void Server_Roll_Implementation(float Value);

	UFUNCTION(Server, unreliable)
	void Server_SetLinearVelocity(float Value);
	void Server_SetLinearVelocity_Implementation(float Value);
	
	UFUNCTION(Server, unreliable)
	void Server_OnLaserFire();
	void Server_OnLaserFire_Implementation();

	UFUNCTION()
	void PowerWithdraw();

	UFUNCTION()
	void BulletWithdraw();

	UFUNCTION(BlueprintCallable, Category = "Counter")
	float GetPower() const;

	UFUNCTION(BlueprintCallable, Category = "Counter")
	float GetMaxPower() const;

	UFUNCTION(BlueprintCallable, Category = "Counter")
	uint8 GetAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Counter")
	TEnumAsByte<EPowerMode> GetPowerMode() const;

	UFUNCTION()
	void HandleLaser();

	UFUNCTION(BlueprintCallable, Category = "Counter")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Counter")
	float GetMaxHealth() const;	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnKillZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};




