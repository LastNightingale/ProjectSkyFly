// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyJetPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Instruments/SkyFlyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Components/ProgressBar.h"
#include "Instruments/SkyFlyGameStateBase.h"
#include "Player/SkyFlyPlayerController.h"

class ASkyFlyPlayerController;
// Sets default values
ASkyFlyJetPawn::ASkyFlyJetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	bReplicates = true;
	
	JetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Jet Mesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Point"));
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));

	SetRootComponent(JetMesh);

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	

	Camera->SetupAttachment(SpringArm);

	SpawnPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnPoint->SetRelativeLocation({ 0.f, 0.f, 0.f });

	HealthBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBarWidget->SetRelativeLocation({ 0.f, 0.f, 50.f });

	Thrust = MaxThrust / 2.f;
	TurnSpeed = 75.f;

	GunOffset = FVector(25.0f, 0.0f, -25.0f);
	
}

// Called when the game starts or when spawned
void ASkyFlyJetPawn::BeginPlay()
{
	Super::BeginPlay();	

	Cast<UEnemyHPWidget>(HealthBarWidget->GetWidget())->PlayerRef = this;
	Cast<UEnemyHPWidget>(HealthBarWidget->GetWidget())->HealthBar->FillColorAndOpacity = HPColor;
	

	if(ASkyFlyPlayerController* PC = GetController<ASkyFlyPlayerController>())
	{
		PlayerHUD = PC->GetHUD<ASkyFlyHUD>();
		PlayerController = GetController<ASkyFlyPlayerController>();
	}
	
	if(IsLocallyControlled())
	{
		PlayerController->SetupHUD();
	}	

	JetMesh->OnComponentBeginOverlap.AddDynamic(this, &ASkyFlyJetPawn::OnKillZoneEnter);
	JetMesh->OnComponentHit.AddDynamic(this, &ASkyFlyJetPawn::OnHit);
}

void ASkyFlyJetPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	if(PlayerController && PlayerController->IsLocalController())
	{		
		PlayerController->EndHUD();
	}
	
	Super::EndPlay(EndPlayReason);

	
}

void ASkyFlyJetPawn::SetHPColor()
{
	if(UEnemyHPWidget* HPWidget = Cast<UEnemyHPWidget>(HealthBarWidget->GetWidget()))
	{
		HPWidget->HealthBar->SetFillColorAndOpacity(HPColor);
	}		
}

// Called every frame
void ASkyFlyJetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsLocallyControlled() && UGameplayStatics::GetPlayerPawn(GetWorld(),0))
	{
		const FVector Location = HealthBarWidget->GetComponentLocation();		
		const FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Location,
			UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetActorLocation());
		HealthBarWidget->SetWorldRotation(PlayerRot);
	}
	
	AddActorLocalOffset(ForwardVelocity * DeltaTime,  true);

	AddActorLocalRotation(FRotator(CurrentPitchSpeed * DeltaTime, CurrentYawSpeed * DeltaTime,
		CurrentRollSpeed * DeltaTime));
}

// Called to bind functionality to input
void ASkyFlyJetPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Thrust", this, &ASkyFlyJetPawn::JetThrust);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASkyFlyJetPawn::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASkyFlyJetPawn::MoveRight);
	PlayerInputComponent->BindAxis("Roll", this, &ASkyFlyJetPawn::Roll);

	PlayerInputComponent->BindAction("OnBulletFire", IE_Pressed, this, &ASkyFlyJetPawn::OnBulletFire);
	PlayerInputComponent->BindAction("ChangeMode", IE_Pressed, this, &ASkyFlyJetPawn::ChangeMode);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ASkyFlyJetPawn::OnPause);
	PlayerInputComponent->BindAction("OpenPlayerPanel", IE_Pressed, this, &ASkyFlyJetPawn::OpenPlayerPanel);
}

void ASkyFlyJetPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyJetPawn, Thrust);
	DOREPLIFETIME(ASkyFlyJetPawn, ForwardVelocity);
	DOREPLIFETIME(ASkyFlyJetPawn, Ammo);
	DOREPLIFETIME(ASkyFlyJetPawn, Power);
	DOREPLIFETIME(ASkyFlyJetPawn, Health);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentLaserState);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentPowerMode);
	DOREPLIFETIME(ASkyFlyJetPawn, PlayerHPWidget);
	DOREPLIFETIME(ASkyFlyJetPawn, HPColor);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentPitchSpeed);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentRollSpeed);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentYawSpeed);
}

void ASkyFlyJetPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.012f));
}

void ASkyFlyJetPawn::JetThrust(float value)
{
	if(HasAuthority())
	{
		if (!FMath::IsNearlyZero(value))
		{
			float Res = FMath::Lerp(-MaxThrust, MaxThrust, (value + 1.f) / 2.f);
			Thrust = FMath::FInterpTo(Thrust, Res, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.25);
		}
		
		ForwardVelocity = FVector(Thrust, 0.f, 0.f);
	}	
	else
	{
		Server_SetLinearVelocity(value);
	}	
}

void ASkyFlyJetPawn::MoveUp(float Value)
{
	if (!HasAuthority())
	{
		Server_MoveUp(Value);
	}
	else
	{
		float TargetPitchSpeed = (Value * TurnSpeed);
		
		TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.1f + FMath::Abs(CurrentRollSpeed) * -0.1f);

		CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
			
	}
}

void ASkyFlyJetPawn::MoveRight(float Value)
{	
	if (!HasAuthority())
	{
		Server_MoveRight(Value);
	}
	else
	{
		float TargetYawSpeed = (Value * TurnSpeed * 1.f);
		
		TargetYawSpeed += (FMath::Abs(CurrentRollSpeed) * -0.1f + FMath::Abs(CurrentPitchSpeed) * -0.1f);
		
		CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	}
}

void ASkyFlyJetPawn::Roll(float Value)
{
	if (!HasAuthority())
	{		
		Server_Roll(Value);
	}
	else
	{
		float TargetRollSpeed = (Value * TurnSpeed);
		
		TargetRollSpeed += (FMath::Abs(CurrentYawSpeed) * -0.1f + FMath::Abs(CurrentPitchSpeed) * -0.1f);

		CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	}
}

void ASkyFlyJetPawn::OnBulletFire()
{
	if(bIsGhosted)
		return;

	if (!CurrentPowerMode)
	{
		if (Ammo > 0 && BulletClass)
		{
			const FRotator SpawnRotation = GetControlRotation();

			const FVector SpawnLocation = SpawnPoint->GetComponentLocation() + GetActorRotation().RotateVector(GunOffset);

			const FVector Direction = GetActorForwardVector().RotateAngleAxis(35.f, GetActorRightVector()) * 5000.f;

			if(HasAuthority())
			{
				SpawnBullet(SpawnLocation, SpawnRotation, Direction);
			}	
			else
			{
				Server_OnBulletFire(SpawnLocation, SpawnRotation, Direction);
			}
		}
	}
	else
	{
		if (Power > 0 && LaserClass)
		{
			if (!HasAuthority())
			{
				Server_OnLaserFire();
			}
			
			CurrentLaserState = (CurrentLaserState == ELaserState::FireOff ? ELaserState::FireOn :
				ELaserState::FireOff);
			HandleLaser();		
		}
	}
}

void ASkyFlyJetPawn::ChangeMode()
{
	if (!HasAuthority() && CurrentLaserState) 
	{
		Server_OnLaserFire();
	}
	CurrentPowerMode = (CurrentPowerMode == EPowerMode::Off ? EPowerMode::On : EPowerMode::Off); 
	if (CurrentLaserState)
		CurrentLaserState = ELaserState::FireOff; 	
	HandleLaser();	

	if (!PlayerController)
		return;

	PlayerController->SwitcherWidget->UISwitcher->SetActiveWidgetIndex(CurrentPowerMode);

}

void ASkyFlyJetPawn::SpawnLaser()
{	
	if (Laser)
		return;

	const FRotator SpawnRotation = GetControlRotation();

	const FVector SpawnLocation = SpawnPoint->GetComponentLocation();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	Laser = GetWorld()->SpawnActor<ASkyShiftLaser>(LaserClass, SpawnLocation, SpawnRotation, SpawnParameters);

	Laser->Base = SpawnPoint;

	Laser->SpawnLaser();

	GetWorldTimerManager().SetTimer(LaserTimerHandle, this, &ASkyFlyJetPawn::PowerWithdraw, 0.1f, true, 0.f);
	
}

void ASkyFlyJetPawn::DestroyLaser()
{
	if (!Laser)
		return;

	GetWorldTimerManager().ClearTimer(LaserTimerHandle);
	Laser->DestroyLaser();
	Laser->Destroy();
	Laser = nullptr;
}

void ASkyFlyJetPawn::OnPause()
{	
	if (!PlayerController)
		return;

	PlayerController->SetUI(EUIMode::UI_PauseMenu);	
}

void ASkyFlyJetPawn::OpenPlayerPanel()
{
	PlayerController->OpenPlayerList();
}

void ASkyFlyJetPawn::ClosePlayerPanel()
{
	PlayerHUD->ClosePlayerList();
}

void ASkyFlyJetPawn::SetGhostedPawn()
{
	bIsGhosted = true;
	JetMesh->SetOnlyOwnerSee(true);	
	HealthBarWidget->SetVisibility(false);

	JetMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	JetMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	JetMesh->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	JetMesh->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	if(HasAuthority())
		GetWorld()->GetGameState<ASkyFlyGameStateBase>()->CheckStateOfPlayers();
}

void ASkyFlyJetPawn::RestorePower(const float Value)
{
	(MaxPower - Value > Power) ? Power += Value : Power = MaxPower;
}

void ASkyFlyJetPawn::RestoreHealth(const float Value)
{
	(MaxHealth - Value > Health) ? Health += Value : Health = MaxHealth;
}

void ASkyFlyJetPawn::RestoreAmmo(const uint8 Value)
{
	(10 - Value > Ammo) ? Ammo += Value : Ammo = 10;
}

void ASkyFlyJetPawn::SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	ASkyShiftBullet* Bullet = GetWorld()->SpawnActor<ASkyShiftBullet>(BulletClass, SpawnLocation, SpawnRotation);


	if (!Bullet)
		return;

	Bullet->SetOwner(this);
	Bullet->BulletMesh->MoveIgnoreActors.Add(GetInstigator());
	Bullet->BulletMesh->MoveIgnoreActors.Add(this);		

	Bullet->SetVelocity(Direction);

	BulletWithdraw();
}

void ASkyFlyJetPawn::Server_OnBulletFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{	
	SpawnBullet(SpawnLocation, SpawnRotation, Direction);
}

void ASkyFlyJetPawn::Server_SetRotation_Implementation(FVector Direction, float value)
{
	JetMesh->AddTorqueInDegrees(Direction * value, NAME_None, true);
}

void ASkyFlyJetPawn::Server_MoveUp_Implementation(float Value)
{
	float TargetPitchSpeed = (Value * TurnSpeed);

	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.1f + FMath::Abs(CurrentRollSpeed) * -0.1f);

	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ASkyFlyJetPawn::Server_MoveRight_Implementation(float Value)
{
	float TargetYawSpeed = (Value * TurnSpeed * 1.f);

	TargetYawSpeed += (FMath::Abs(CurrentRollSpeed) * -0.1f + FMath::Abs(CurrentPitchSpeed) * -0.1f);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ASkyFlyJetPawn::Server_Roll_Implementation(float Value)
{
	float TargetRollSpeed = (Value * TurnSpeed);

	TargetRollSpeed += (FMath::Abs(CurrentYawSpeed) * -0.1f + FMath::Abs(CurrentPitchSpeed) * -0.1f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ASkyFlyJetPawn::Server_SetLinearVelocity_Implementation(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		float Res = FMath::Lerp(-MaxThrust, MaxThrust, (Value + 1.f) / 2.f);
		Thrust = FMath::FInterpTo(Thrust, Res, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.25);
	}
		
	ForwardVelocity = FVector(Thrust, 0.f, 0.f);
}

void ASkyFlyJetPawn::Server_OnLaserFire_Implementation()
{	
	CurrentLaserState = (CurrentLaserState == ELaserState::FireOff ? ELaserState::FireOn : ELaserState::FireOff);
	HandleLaser();
}

void ASkyFlyJetPawn::PowerWithdraw()
{
	Power -= 1.5;
	HandleLaser();
}

void ASkyFlyJetPawn::BulletWithdraw()
{
	Ammo -= 1;
}

float ASkyFlyJetPawn::GetPower() const 
{
	return Power;
}

float ASkyFlyJetPawn::GetMaxPower() const
{
	return MaxPower;
}

uint8 ASkyFlyJetPawn::GetAmmo() const
{
	return Ammo;
}

TEnumAsByte<EPowerMode> ASkyFlyJetPawn::GetPowerMode() const
{
	return CurrentPowerMode;
}

void ASkyFlyJetPawn::HandleLaser()
{
	if (CurrentLaserState && Power > 0.f)
		SpawnLaser();
	else DestroyLaser();
}

float ASkyFlyJetPawn::GetHealth() const
{
	return Health;
}

float ASkyFlyJetPawn::GetMaxHealth() const
{
	return MaxHealth;
}

float ASkyFlyJetPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	(Health > DamageAmount) ? Health = Health - DamageAmount : Health = 0.f;

	if(PlayerHPWidget)
		PlayerHPWidget->SetHealth(Health, MaxHealth);

	if(!Health)
		SetGhostedPawn();
	
	return DamageAmount;
}

void ASkyFlyJetPawn::OnKillZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageEvent DamageEvent;
	if (ASkyShiftLaser* LaserCollided = Cast<ASkyShiftLaser>(OtherActor))
	{
		if(LaserCollided->GetOwner() != this)
			this->TakeDamage(LaserCollided->DamagePerTick, DamageEvent, LaserCollided->GetInstigatorController(), LaserCollided);
	}
}

void ASkyFlyJetPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	if (ASkyShiftBullet* Bullet = Cast<ASkyShiftBullet>(OtherActor))
	{
		const FDamageEvent DamageEvent;
		this->TakeDamage(Bullet->Damage, DamageEvent, Bullet->GetInstigatorController(), Bullet);
		Bullet->Destroy();
	}
}




