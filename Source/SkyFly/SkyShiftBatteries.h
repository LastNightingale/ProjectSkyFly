// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SkyShiftBatteries.generated.h"

UCLASS()
class SKYFLY_API ASkyShiftBatteries : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyShiftBatteries();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BatteryMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USphereComponent* BatteryBoundaries;

	UPROPERTY(BlueprintReadOnly)
	APawn* PickingPawn = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void WasPicked();
	virtual void WasPicked_Implementation();

	UFUNCTION(BlueprintPure, Category = "Player")
	APawn* GetPickingPawn();

};
