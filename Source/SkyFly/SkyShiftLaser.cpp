

#include "SkyShiftLaser.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASkyShiftLaser::ASkyShiftLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	
}

// Called when the game starts or when spawned
void ASkyShiftLaser::BeginPlay()
{
	Super::BeginPlay();	
	
}

// Called every frame
void ASkyShiftLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	Laser->SetBeamSourcePoint(0, Base->GetComponentLocation(), 0);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Base->GetComponentLocation(),
		Base->GetComponentLocation() + Base->GetForwardVector() * LaserPower, ECollisionChannel::ECC_Visibility))
	{	

		Laser->SetBeamEndPoint(0, HitResult.Location);
		if (LaserHit == nullptr)
		{
			const FTransform SpawnTransform;
			LaserHit = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserHitParticleClass, SpawnTransform);
			LaserHit->SetIsReplicated(true);
		}
		LaserHit->SetVisibility(true);
		LaserHit->SetWorldLocation(HitResult.Location);
	}
	else
	{		
		Laser->SetBeamEndPoint(0, Base->GetComponentLocation() + Base->GetForwardVector() * LaserPower);
		if (LaserHit != nullptr)
		{
			LaserHit->SetVisibility(false);
		}
	}

}

void ASkyShiftLaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
	/*DOREPLIFETIME(ASkyShiftLaser, Laser);
	DOREPLIFETIME(ASkyShiftLaser, LaserHit);*/
}

void ASkyShiftLaser::SpawnLaser()
{
	Laser = UGameplayStatics::SpawnEmitterAttached(LaserParticleClass, Base);
	Laser->SetIsReplicated(true);
}

void ASkyShiftLaser::DestroyLaser()
{
	if (LaserHit != nullptr) LaserHit->DestroyComponent();
	if (Laser != nullptr) Laser->DestroyComponent();
}


