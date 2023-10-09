// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnVolume.generated.h"

UENUM()
enum class ECollectables : uint8
{
	Battery = 0,
	MedKit = 1,
	Ammo = 2
};

UCLASS()
class SKYFLY_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* SpawnBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	TMap<ECollectables, TSubclassOf<ACollectable>> ItemsToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	float SpawnDelayRangeLow;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	float SpawnDelayRangeHigh;

	float SpawnDelay;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UBoxComponent* GetSpawnBox() const;

	UFUNCTION(BlueprintCallable)
	FVector GetRandomPointInVolume() const ;

	UFUNCTION(BlueprintCallable)
	ECollectables GetRandomCollectableType() const;
	
	void SpawnCollectable();

};
