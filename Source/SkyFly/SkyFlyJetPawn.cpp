// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyJetPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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
	//bAlwaysRelevant = true;

	JetMesh = CreateDefaultSubobject<UStaticMeshComponent>("Jet Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Camera Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	MovingComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Movement Component");

	SetRootComponent(JetMesh);

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	

	Camera->SetupAttachment(SpringArm);
	//Camera->SetRelativeLocation(FVector(0.f, 0.f, -100.f));

	Thrust = MaxThrust / 2.f;

	
	/*bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;*/
}

// Called when the game starts or when spawned
void ASkyFlyJetPawn::BeginPlay()
{
	Super::BeginPlay();

	CreateWidget<UUserWidget>(GetWorld(), WidgetBP[UIMode::UI_PowerOff])->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(this, 0));

	
}

// Called every frame
void ASkyFlyJetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && !IsLocallyControlled())
	{
		/*UE_LOG(LogTemp, Warning, TEXT("Vector: %s"), *ForvardVelocity.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Vector: %f"), Thrust);*/
	}
	

	JetMesh->SetPhysicsLinearVelocity(ForvardVelocity);	

	/*if (bOnLaserFire && Power > 0.f)
		SpawnLaser();
	else if (Laser != nullptr)
		DestroyLaser();*/
	
	
	
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
	DOREPLIFETIME(ASkyFlyJetPawn, bOnLaserFire);
	DOREPLIFETIME(ASkyFlyJetPawn, bInPowerMode);
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
	else
	{
		if (IsLocallyControlled())
		{
			JetMesh->AddTorqueInDegrees(GetActorRightVector() * value * -20.f, NAME_None, true);
		}
	}
}

void ASkyFlyJetPawn::MoveRight(float value)
{	
	if (!HasAuthority())
	{
		Server_SetRotation(GetActorUpVector(), value * 20.f);
	}
	else
	{
		if (IsLocallyControlled())
		{
			JetMesh->AddTorqueInDegrees(GetActorUpVector() * value * 20.f, NAME_None, true);
		}
	}
}

void ASkyFlyJetPawn::Roll(float value)
{

	if (!HasAuthority())
	{		
		Server_SetRotation(GetActorForwardVector(), value * 20.f);
	}
	else
	{
		if (IsLocallyControlled())
		{
			JetMesh->AddTorqueInDegrees(GetActorForwardVector() * value * 20.f, NAME_None, true);
		}
	}
}

void ASkyFlyJetPawn::OnBulletFire()
{

	if (BulletClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();

		const FVector SpawnLocation = JetMesh->K2_GetComponentLocation();

		UWorld* World = GetWorld();

		if (World != NULL)
		{
			if (!bInPowerMode)
			{
				if (Ammo > 0)
				{
					ASkyShiftBullet* Bullet = World->SpawnActor<ASkyShiftBullet>(BulletClass, SpawnLocation, SpawnRotation);


					FVector NewVelocity = GetActorForwardVector() * 5000.f;

					//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorForwardVector() * 5000.f, FColor::Emerald, true, -1, 0, 10);

					Bullet->SetVelocity(NewVelocity);

					BulletWithdraw();

					if (!HasAuthority())
					{
						Server_OnBulletFire(SpawnLocation, SpawnRotation, GetActorForwardVector() * 5000.f);
					}
				}
				
			}
			else
			{		
				if (Power > 0)
				{
					if (!HasAuthority())
					{
						Server_OnLaserFire();
					}
					bOnLaserFire = !bOnLaserFire;
					HandleLaser();
				}				
			}
		}
	}
}

void ASkyFlyJetPawn::ChangeMode()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	if (!HasAuthority() && bOnLaserFire)
	{
		Server_OnLaserFire();
	}
	bInPowerMode = !bInPowerMode;
	if (bOnLaserFire) bOnLaserFire = !bOnLaserFire;
	UUserWidget* CurrentWidget = nullptr;
	if(!bInPowerMode) CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetBP[UIMode::UI_PowerOff]);
	else CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetBP[UIMode::UI_PowerOn]);
	CurrentWidget->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(this, 0));
	HandleLaser();	

}

void ASkyFlyJetPawn::SpawnLaser()
{	
	if (Laser == nullptr)
	{
		const FRotator SpawnRotation = GetControlRotation();

		const FVector SpawnLocation = JetMesh->K2_GetComponentLocation();

		Laser = GetWorld()->SpawnActor<ASkyShiftLaser>(LaserClass, SpawnLocation, SpawnRotation);

		Laser->Base = JetMesh;

		Laser->SpawnLaser();

		GetWorldTimerManager().SetTimer(LaserTimerHandle, this, &ASkyFlyJetPawn::PowerWithdraw, 0.1f, true, 0.f);

	}
	
}

void ASkyFlyJetPawn::DestroyLaser()
{
	/*if (!HasAuthority() && !IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserTriedDestroyed"));
		UE_LOG(LogTemp, Warning, TEXT("Laser: %d"), (Laser != nullptr));
	}	*/
	if (Laser != nullptr)
	{
		GetWorldTimerManager().ClearTimer(LaserTimerHandle);
		Laser->DestroyLaser();
		Laser->Destroy();
		Laser = nullptr;

		/*if(!HasAuthority() && !IsLocallyControlled())
		UE_LOG(LogTemp, Warning, TEXT("LaserDestroyed"));*/		
	}	
}

void ASkyFlyJetPawn::OnPause()
{
	if (WidgetBP[UIMode::UI_PauseMenu] != nullptr)
	{
		UUserWidget* PauseWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetBP[UIMode::UI_PauseMenu]);

		if (PauseWidget != nullptr)
		{
			PauseWidget->AddToViewport();
			APlayerController* JetPlayer = UGameplayStatics::GetPlayerController(this, 0);
			JetPlayer->SetShowMouseCursor(true);
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(JetPlayer, PauseWidget);
		}
	}
}

bool ASkyFlyJetPawn::Server_OnBulletFire_Validate(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	return true;
}

void ASkyFlyJetPawn::Server_OnBulletFire_Implementation(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
	ASkyShiftBullet* Bullet = GetWorld()->SpawnActor<ASkyShiftBullet>(BulletClass, SpawnLocation, SpawnRotation);


	Bullet->SetVelocity(Direction);
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
	bOnLaserFire = !bOnLaserFire;
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

bool ASkyFlyJetPawn::GetPowerMode()
{
	return bInPowerMode;
}

void ASkyFlyJetPawn::HandleLaser()
{
	if (bOnLaserFire && Power > 0.f)
		SpawnLaser();
	else DestroyLaser();
}




