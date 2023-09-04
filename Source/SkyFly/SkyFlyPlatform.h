// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SkyFlyPlatform.generated.h"

UCLASS()
class SKYFLY_API ASkyFlyPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyFlyPlatform();

	/*UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* LeftWallMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RightWallMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ForwardWallMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BackWallMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CeilingMesh;*/

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* Edges[5];

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UENUM()
enum Airspace
{
	/*Platform,*/
	LeftEdge,
	RightEdge,
	ForwardEdge,
	BackEdge,
	Ceiling
};
