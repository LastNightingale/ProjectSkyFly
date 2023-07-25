// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyJetPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASkyFlyJetPawn::ASkyFlyJetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JetMesh = CreateDefaultSubobject<UStaticMeshComponent>("Jet Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Camera Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

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
	
}

// Called every frame
void ASkyFlyJetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (ForvardVelocity.Size() < GravityThreshHold)
	{
		FVector UnrotatedVector = GetActorRotation().UnrotateVector(GetActorUpVector());
		float ToLerp = 1.0 - (ForvardVelocity.Size() / GravityThreshHold);
		UpVelocity = UnrotatedVector * FMath::Lerp(0.f, -980.f, ToLerp);
	}
	else
	{
		UpVelocity = FMath::VInterpTo(UpVelocity, FVector::ZeroVector, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 20.f);
	}*/
	
	//FVector UnrotatedVector = GetActorRotation().UnrotateVector(GetActorUpVector());
	//float ToLerp = 1.0 - (ForvardVelocity.Size() / GravityThreshHold);
	////UpVelocity = UnrotatedVector * FMath::Lerp(0.f, -980.f, ToLerp);

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
}

void ASkyFlyJetPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkyFlyJetPawn, Thrust);
	DOREPLIFETIME(ASkyFlyJetPawn, ForvardVelocity);
}

void ASkyFlyJetPawn::JetThrust(float value)
{
	if (!FMath::IsNearlyZero(value))
	{
		float Res = FMath::Lerp(-MaxThrust, MaxThrust, (value + 1.f) / 2.f);
		UE_LOG(LogTemp, Warning, TEXT("Lerp: %f"), Res);
		Thrust = FMath::FInterpTo(Thrust, Res, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.25);
		UE_LOG(LogTemp, Warning, TEXT("Thrust: %f"), Thrust);
	}
	ForvardVelocity = GetActorForwardVector() * Thrust;
}

void ASkyFlyJetPawn::MoveUp(float value)
{
	JetMesh->AddTorqueInDegrees(GetActorRightVector() * value * -20.f, NAME_None, true);
}

void ASkyFlyJetPawn::MoveRight(float value)
{
	JetMesh->AddTorqueInDegrees(GetActorUpVector() * value * 20.f, NAME_None, true);
}

void ASkyFlyJetPawn::Roll(float value)
{
	JetMesh->AddTorqueInDegrees(GetActorForwardVector() * value * 20.f, NAME_None, true);
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
			ASkyShiftBullet* Bullet = World->SpawnActor<ASkyShiftBullet>(BulletClass, SpawnLocation, SpawnRotation);


			FVector NewVelocity = GetActorForwardVector() * 5000.f;

			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorForwardVector() * 5000.f, FColor::Emerald, true, -1, 0, 10);

			Bullet->SetVelocity(NewVelocity);

			if (!HasAuthority())
			{
				Server_OnBulletFire(SpawnLocation, SpawnRotation, GetActorForwardVector() * 5000.f);
			}
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



