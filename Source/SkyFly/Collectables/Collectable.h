// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/SkyFlyJetPawn.h"
#include "Components/SphereComponent.h"
#include "Collectable.generated.h"

UCLASS()
class SKYFLY_API ACollectable : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BatteryMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USphereComponent* BatteryBoundaries;

	UPROPERTY(BlueprintReadOnly)
	ASkyFlyJetPawn* PickingPawn = nullptr;



	// Sets default values for this actor's properties
	ACollectable();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void WasPicked();
	void WasPicked_Implementation();

	UFUNCTION(BlueprintPure, Category = "Player")
	ASkyFlyJetPawn* GetPickingPawn();

	virtual void RestoreValue();

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
