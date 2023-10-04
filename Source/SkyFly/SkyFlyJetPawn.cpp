// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyJetPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SkyFlyCanvas.h"
#include "Components/CanvasPanelSlot.h"
#include "EngineUtils.h"
#include "SkyFlyHUD.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASkyFlyJetPawn::ASkyFlyJetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(true);
	bReplicates = true;
	
	JetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Jet Mesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpawnPoint = CreateDefaultSubobject<USphereComponent>(TEXT("Spawn Point"));
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));

	SetRootComponent(JetMesh);

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 200.0f;
	//SpringArm->TargetArmLength = 500.f;
	//SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	SpringArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	

	Camera->SetupAttachment(SpringArm);
	//Camera->SetRelativeLocation(FVector(0.f, 0.f, -100.f));

	SpawnPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnPoint->SetRelativeLocation({ 0.f, 0.f, 0.f });

	HealthBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HealthBarWidget->SetRelativeLocation({ 0.f, 0.f, 50.f });

	Thrust = MaxThrust / 2.f;	

	GunOffset = FVector(25.0f, 0.0f, -25.0f);
	//JetMesh->OnComponent.AddDynamic(this, &ASkyFlyJetPawn::OnHit);	
	
}

// Called when the game starts or when spawned
void ASkyFlyJetPawn::BeginPlay()
{
	Super::BeginPlay();	

	Cast<UEnemyHPWidget>(HealthBarWidget->GetWidget())->PlayerRef = this;
	

	if(GetController<APlayerController>())
	PlayerHUD = GetController<APlayerController>()->GetHUD<ASkyFlyHUD>();		
	

	JetMesh->OnComponentBeginOverlap.AddDynamic(this, &ASkyFlyJetPawn::OnKillZoneEnter);
	JetMesh->OnComponentHit.AddDynamic(this, &ASkyFlyJetPawn::OnHit);
}

// Called every frame
void ASkyFlyJetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (HasAuthority() && !IsLocallyControlled())
	//{
	//	/*UE_LOG(LogTemp, Warning, TEXT("Vector: %s"), *ForvardVelocity.ToString());
	//	UE_LOG(LogTemp, Warning, TEXT("Vector: %f"), Thrust);*/
	//}
	
	/*if (IsLocallyControlled())
	{

		FVector2D outVector;		
		for (TActorIterator<ASkyFlyJetPawn> It(GetWorld()); It; ++It)
		{
			if (!It->IsLocallyControlled())
			{
				UGameplayStatics::ProjectWorldToScreen(GetController<APlayerController>(), It->GetActorLocation(), outVector, false);
				outVector *= FMath::Pow(UWidgetLayoutLibrary::GetViewportScale(GetWorld()), -1);
				if (!It->PlayerHPWidget)
					continue;

				//UE_LOG(LogTemp, Warning, TEXT("OutVector: %s"), *outVector.ToString());

				if (outVector.IsZero())
					It->PlayerHPWidget->Visibility = ESlateVisibility::Hidden;
				else
				{
					It->PlayerHPWidget->Visibility = ESlateVisibility::Visible;
					Cast<UCanvasPanelSlot>(It->PlayerHPWidget->Slot)->SetPosition(outVector);
				}			
			}
		}
	}*/
	

	JetMesh->SetPhysicsLinearVelocity(ForvardVelocity);		
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
}

void ASkyFlyJetPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyJetPawn, Thrust);
	DOREPLIFETIME(ASkyFlyJetPawn, ForvardVelocity);
	DOREPLIFETIME(ASkyFlyJetPawn, Ammo);
	DOREPLIFETIME(ASkyFlyJetPawn, Power);
	DOREPLIFETIME(ASkyFlyJetPawn, Health);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentLaserState);
	DOREPLIFETIME(ASkyFlyJetPawn, CurrentPowerMode);
	DOREPLIFETIME(ASkyFlyJetPawn, PlayerHPWidget);
}

void ASkyFlyJetPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.012f));
}

void ASkyFlyJetPawn::JetThrust(float value)
{
	if (!FMath::IsNearlyZero(value))
	{
		float Res = FMath::Lerp(-MaxThrust, MaxThrust, (value + 1.f) / 2.f);
		/*UE_LOG(LogTemp, Warning, TEXT("Lerp: %f"), Res);*/
		Thrust = FMath::FInterpTo(Thrust, Res, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.25);
		/*UE_LOG(LogTemp, Warning, TEXT("Thrust: %f"), Thrust);*/
	}
	ForvardVelocity = GetActorForwardVector() * Thrust;

	Server_SetLinearVelocity(ForvardVelocity);
}

void ASkyFlyJetPawn::MoveUp(float value)
{
	if (!HasAuthority())
	{
		Server_SetRotation(GetActorRightVector(), value * -20.f);
	}
	else if (IsLocallyControlled())
	{
		JetMesh->AddTorqueInDegrees(GetActorRightVector() * value * -20.f, NAME_None, true);
	}
}

void ASkyFlyJetPawn::MoveRight(float value)
{	
	if (!HasAuthority())
	{
		Server_SetRotation(GetActorUpVector(), value * 20.f);
	}
	else if (IsLocallyControlled())
	{
		JetMesh->AddTorqueInDegrees(GetActorUpVector() * value * 20.f, NAME_None, true);
	}
}

void ASkyFlyJetPawn::Roll(float value)
{

	if (!HasAuthority())
	{		
		Server_SetRotation(GetActorForwardVector(), value * 20.f);
	}
	else if (IsLocallyControlled())
	{
		JetMesh->AddTorqueInDegrees(GetActorForwardVector() * value * 20.f, NAME_None, true);
	}
}

void ASkyFlyJetPawn::OnBulletFire()
{

	if (!CurrentPowerMode)
	{
		if (Ammo > 0 && BulletClass)
		{
			const FRotator SpawnRotation = GetControlRotation();

			const FVector SpawnLocation = SpawnPoint->GetComponentLocation() + GetActorRotation().RotateVector(GunOffset);

			FVector Direction = GetActorForwardVector().RotateAngleAxis(35.f, GetActorRightVector()) * 5000.f;

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
			
			CurrentLaserState = (CurrentLaserState == LaserState::FireOff ? LaserState::FireOn : LaserState::FireOff);
			HandleLaser();
			
			//bOnLaserFire = !bOnLaserFire;			
		}
	}
}

void ASkyFlyJetPawn::ChangeMode()
{
	//UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld()); //убрать інший юай з екрана 
	if (!HasAuthority() && CurrentLaserState) //якщо лазер увімкнений на клієнті при переході в режим куль вирубити лазер
	{
		Server_OnLaserFire();
	}
	CurrentPowerMode = (CurrentPowerMode == PowerMode::Off ? PowerMode::On : PowerMode::Off); //перемкнути режим
	if (CurrentLaserState)
		CurrentLaserState = LaserState::FireOff; //і вирубити лазер	
	HandleLaser();	//якщо лазер увімкнений вирубити

	if (!PlayerHUD)
		return;

	PlayerHUD->UISwitcher->SetActiveWidgetIndex(CurrentPowerMode);

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
	/*if (!HasAuthority() && !IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserTriedDestroyed"));
		UE_LOG(LogTemp, Warning, TEXT("Laser: %d"), (Laser != nullptr));
	}	*/
	if (!Laser)
		return;

	GetWorldTimerManager().ClearTimer(LaserTimerHandle);
	Laser->DestroyLaser();
	Laser->Destroy();
	Laser = nullptr;
}

void ASkyFlyJetPawn::OnPause()
{	
	if (!PlayerHUD)
		return;

	PlayerHUD->SetUI(UIMode::UI_PauseMenu);	
}

void ASkyFlyJetPawn::RestorePower(float Value)
{
	if (MaxPower - Value > Power)
		Power += Value;
	else
		Power = MaxPower;
}

void ASkyFlyJetPawn::RestoreHealth(float Value)
{
	if (MaxHealth - Value > Health)
		Health += Value;
	else
		Health = MaxHealth;
}

void ASkyFlyJetPawn::RestoreAmmo(uint8 Value)
{
	if (10 - Value > Ammo)
		Ammo += Value;
	else
		Ammo = 10;
}

void ASkyFlyJetPawn::SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	ASkyShiftBullet* Bullet = GetWorld()->SpawnActor<ASkyShiftBullet>(BulletClass, SpawnLocation, SpawnRotation);

	//UE_LOG(LogTemp, Warning, TEXT("BulletFired"));

	/*DrawDebugLine(GetWorld(), SpawnLocation + GetActorRotation().RotateVector(GunOffset),
		GetActorForwardVector().RotateAngleAxis(35.f, GetActorRightVector()) * 5000.f, FColor::Emerald, true, -1, 0, 10);*/

	if (!Bullet)
		return;

	Bullet->SetOwner(this);
	Bullet->BulletMesh->MoveIgnoreActors.Add(GetInstigator());
	Bullet->BulletMesh->MoveIgnoreActors.Add(this);							

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorForwardVector() * 5000.f, FColor::Emerald, true, -1, 0, 10);

	Bullet->SetVelocity(Direction);

	BulletWithdraw();
}

bool ASkyFlyJetPawn::Server_OnBulletFire_Validate(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	return true;
}

void ASkyFlyJetPawn::Server_OnBulletFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	/*ASkyShiftBullet* Bullet = GetWorld()->SpawnActor<ASkyShiftBullet>(BulletClass, SpawnLocation, SpawnRotation);
	
	UE_LOG(LogTemp, Warning, TEXT("RPCBullet"));
	
	Bullet->SetOwner(this);
	Bullet->BulletMesh->MoveIgnoreActors.Add(Bullet->GetOwner());
	Bullet->BulletMesh->MoveIgnoreActors.Add(GetInstigator());
	Bullet->SetVelocity(Direction);*/
	SpawnBullet(SpawnLocation, SpawnRotation, Direction);
}

bool ASkyFlyJetPawn::Server_SetRotation_Validate(FVector Direction, float value)
{
	return true;
}

void ASkyFlyJetPawn::Server_SetRotation_Implementation(FVector Direction, float value)
{
	JetMesh->AddTorqueInDegrees(Direction * value, NAME_None, true);
}

bool ASkyFlyJetPawn::Server_SetLinearVelocity_Validate(FVector NewVelocity)
{
	return true;
}

void ASkyFlyJetPawn::Server_SetLinearVelocity_Implementation(FVector NewVelocity)
{
	ForvardVelocity = NewVelocity;
}

bool ASkyFlyJetPawn::Server_OnLaserFire_Validate()
{
	return true;
}

void ASkyFlyJetPawn::Server_OnLaserFire_Implementation()
{	
	CurrentLaserState = (CurrentLaserState == LaserState::FireOff ? LaserState::FireOn : LaserState::FireOff);
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

float ASkyFlyJetPawn::GetPower()
{
	return Power;
}

float ASkyFlyJetPawn::GetMaxPower()
{
	return MaxPower;
}

uint8 ASkyFlyJetPawn::GetAmmo()
{
	return Ammo;
}

TEnumAsByte<PowerMode> ASkyFlyJetPawn::GetPowerMode()
{
	return CurrentPowerMode;
}

void ASkyFlyJetPawn::HandleLaser()
{
	if (CurrentLaserState && Power > 0.f)
		SpawnLaser();
	else DestroyLaser();
}

float ASkyFlyJetPawn::GetHealth()
{
	return Health;
}

float ASkyFlyJetPawn::GetMaxHealth()
{
	return MaxHealth;
}

float ASkyFlyJetPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("Health: %u"), Health);
	if (Health > DamageAmount)
		Health = Health - DamageAmount;
	else 
		Health = 0.f;

	if(PlayerHPWidget)
	PlayerHPWidget->SetHealth(Health, MaxHealth);
	//if(!IsLocallyControlled())
	
	return DamageAmount;
}

void ASkyFlyJetPawn::OnKillZoneEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*FDamageEvent DamageEvent;
	if (ASkyShiftLaser* LaserCollided = Cast<ASkyShiftLaser>(OtherActor))
	{
		if(LaserCollided->GetOwner() != this)
			this->TakeDamage(LaserCollided->DamagePerTick, DamageEvent, LaserCollided->GetInstigatorController(), LaserCollided);
		//UE_LOG(LogTemp, Warning, TEXT("BulletTime"));
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("Why Don't You Work("));
}

void ASkyFlyJetPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	if (ASkyShiftBullet* Bullet = Cast<ASkyShiftBullet>(OtherActor))
	{
		FDamageEvent DamageEvent;
		this->TakeDamage(Bullet->Damage, DamageEvent, Bullet->GetInstigatorController(), Bullet);
		Bullet->Destroy();
		//UE_LOG(LogTemp, Warning, TEXT("BulletTime"));
	}		
	//else
		//UE_LOG(LogTemp, Warning, TEXT("LaserTime"));
}




