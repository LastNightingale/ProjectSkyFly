// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyPlatform.h"

// Sets default values
ASkyFlyPlatform::ASkyFlyPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*for (auto& space : AirspaceMesh)
	{
		space = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	}*/

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));

	for (int i = 0; i < 2; i++)
	{
		Edges[i] = CreateDefaultSubobject<UBoxComponent>(*(FString::Printf(TEXT("EdgeMesh_%u"), i)));
	}

	SetRootComponent(PlatformMesh);

}

// Called when the game starts or when spawned
void ASkyFlyPlatform::BeginPlay()
{
	Super::BeginPlay();

	FVector Min, Max;

	FTransform Transform;
	//PlatformMesh->GetLocalBounds(Min, Max);
	FVector Size =  PlatformMesh->CalcBounds(Transform).GetBox().GetSize();
	//Max - Min;

	//FVector EMin, EMax;

	FVector ESize = Edges[Airspace::LeftEdge]->GetUnscaledBoxExtent();
	 //= EMax - EMin;

	UE_LOG(LogTemp, Warning, TEXT("Vector: %s"), *ESize.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Vector: %s"), *Size.ToString());

	Edges[Airspace::LeftEdge]->SetRelativeScale3D({ Size.X / ESize.X, Size.Y / ESize.Y, Size.Z / ESize.Z });

	Edges[Airspace::LeftEdge]->SetRelativeRotation({0.f, 0.f, 90.f});

	Edges[Airspace::LeftEdge]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Edges[Airspace::LeftEdge]->SetRelativeLocation({ 0.f, -Size.Y / 2.f - Size.Z / 2.f, Size.X / 2.f + Size.Z / 2.f });
}

// Called every frame
void ASkyFlyPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

