// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));

	SpawnDelayRangeLow = 1.0;
	SpawnDelayRangeHigh = 5.0;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnCollectable,
		SpawnDelay, false);
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBoxComponent* ASpawnVolume::GetSpawnBox() const
{
	return SpawnBox;
}

FVector ASpawnVolume::GetRandomPointInVolume() const 
{
	const FVector SpawnOrigin = SpawnBox->Bounds.Origin;
	const FVector SpawnExtent = SpawnBox->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

ECollectables ASpawnVolume::GetRandomCollectableType() const
{
	return static_cast<ECollectables>(FMath::RandRange(0,2));
}

void ASpawnVolume::SpawnCollectable()
{
	const ECollectables RandomCollectable = GetRandomCollectableType();

	if(!ItemsToSpawn[RandomCollectable])
		return;

	UWorld* World = GetWorld();

	if(!World)
		return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = GetInstigator();

	const FVector SpawnLocation = GetRandomPointInVolume();

	const FRotator SpawnRotation = FRotator(FMath::FRand() * 360.f, FMath::FRand() * 360.f,
		FMath::FRand() * 360.f);

	World->SpawnActor<ACollectable>(ItemsToSpawn[RandomCollectable], SpawnLocation, SpawnRotation, SpawnParameters);

	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnCollectable,
		SpawnDelay, false);	
}

