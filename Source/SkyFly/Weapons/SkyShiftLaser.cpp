

#include "SkyShiftLaser.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SkyFlyJetPawn.h"

// Sets default values
ASkyShiftLaser::ASkyShiftLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	BaseScene = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComponent"));
	LaserHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));	

	SetRootComponent(BaseScene);
	LaserHitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	LaserHitBox->OnComponentBeginOverlap.AddDynamic(this, &ASkyShiftLaser::OnLaserBeginOverlap);
	LaserHitBox->OnComponentEndOverlap.AddDynamic(this, &ASkyShiftLaser::OnLaserEndOverlap);
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

	Laser->SetBeamSourcePoint(0, GetOwner()->GetActorLocation(), 0);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult,GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector().RotateAngleAxis(35.f,
			GetOwner()->GetActorRightVector()) * LaserPower, ECollisionChannel::ECC_Visibility))
	{	
		EndLocation = HitResult.Location;
		if (!LaserHit)
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
		EndLocation =GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector().RotateAngleAxis(35.f, GetOwner()->GetActorRightVector()) * LaserPower;
		if (LaserHit)
		{
			LaserHit->SetVisibility(false);
		}
	}

	Laser->SetBeamEndPoint(0, EndLocation);

	FVector BaseVector = EndLocation - GetOwner()->GetActorLocation();

	FVector Location = BaseVector / 2.f + GetOwner()->GetActorLocation();
	FRotator Rotation = BaseVector.Rotation();
	FVector Scale = (LaserHit && LaserHit->IsVisible() ?
		FVector(FVector::Dist(GetOwner()->GetActorLocation(), EndLocation) / 2.f, 10.f, 10.f)
		: FVector(LaserPower / 2.f, 10.f, 10.f));
	

	SetCollisionBox(Location,
		Rotation,
		Scale);

}

void ASkyShiftLaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASkyShiftLaser::SpawnLaser()
{
	Laser = UGameplayStatics::SpawnEmitterAttached(LaserParticleClass, Base);
	Laser->SetIsReplicated(true);	
}

void ASkyShiftLaser::DestroyLaser()
{
	if (LaserHit) 
		LaserHit->DestroyComponent();
	if (Laser) 
		Laser->DestroyComponent();
}

void ASkyShiftLaser::SetCollisionBox(FVector LocationVector, FRotator RotationRotator, FVector ScaleVector)
{
	LaserHitBox->SetWorldRotation(RotationRotator);
	LaserHitBox->SetBoxExtent(ScaleVector);	
	LaserHitBox->SetWorldLocation(LocationVector);
}

void ASkyShiftLaser::OnLaserBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APawn>(OtherActor) && OtherActor != this->GetOwner())
	{
		if (OtherActor != this->GetOwner())
		{
			PlayerToHit = OtherActor;
			Hit = SweepResult;
			GetWorldTimerManager().SetTimer(LaserDamageHandle, this, &ASkyShiftLaser::ApplyLaserDamage, 0.1f, true, 0.0f);
		}
	}
}

void ASkyShiftLaser::OnLaserEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APawn>(OtherActor) && OtherActor != this->GetOwner())
	{
		if (OtherActor != this->GetOwner())
		{
			GetWorldTimerManager().ClearTimer(LaserDamageHandle);
		}
	}

}

void ASkyShiftLaser::ApplyLaserDamage()
{
	UGameplayStatics::ApplyPointDamage(PlayerToHit, DamagePerTick, GetActorLocation(), Hit, nullptr, this, LaserDamage);
}


