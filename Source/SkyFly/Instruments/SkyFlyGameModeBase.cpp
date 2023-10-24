// Copyright Epic Games, Inc. All Rights Reserved.


#include "SkyFlyGameModeBase.h"

#include "SkyFlyGameStateBase.h"
#include "Player/SkyFlyJetPawn.h"
#include "Player/SkyFlyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetMathLibrary.h"

void ASkyFlyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASkyFlyGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	

	////

	//if(ASkyFlyJetPawn* ToCheck = Cast<ASkyFlyJetPawn>(NewPlayer))
	//{
	//	return;
	//}

	//if (NewPlayer->GetPawn())
	//{
	//	NewPlayer->GetPawn()->Destroy();
	//}

	//FTransform SpawnTransform;
	//TArray<AActor*> FoundEntries;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASkyFlyJetPawn::StaticClass(), FoundEntries);

	//if (FoundEntries.Num() > 0)
	//{
	//	SpawnTransform = FoundEntries[0]->GetActorTransform();
	//}
	//else
	//{
	//	TArray<AActor*> FoundPlayerStarts;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPlayerStarts);
	//	SpawnTransform = FoundPlayerStarts[0]->GetActorTransform();
	//}

	//float TempFloatX = UKismetMathLibrary::RandomBool() ? UKismetMathLibrary::RandomFloatInRange(-50.f, -150.0f) 
	//	: UKismetMathLibrary::RandomFloatInRange(50.f, 150.0f);
	//float TempFloatY = UKismetMathLibrary::RandomFloatInRange(-50.0f, -150.0f);

	//SpawnTransform.SetLocation((SpawnTransform.GetLocation() + FVector(TempFloatX, TempFloatY, 0.f)));

	//FActorSpawnParameters SpawnInfo;
	//SpawnInfo.Owner = NewPlayer;
	//SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//FVector TempLoc = SpawnTransform.GetLocation();
	//FRotator TempRot = SpawnTransform.GetRotation().Rotator();

	//UClass* JetClass = DefaultPawnClass->StaticClass();
	//
	//AActor* TempActor = GetWorld()->SpawnActor(JetClass, &TempLoc, &TempRot, SpawnInfo);
	////ASkyFlyJetPawn* TempActor = Cast<ASkyFlyJetPawn>(GetWorld()->SpawnActor(ASkyFlyJetPawn::StaticClass(), &TempLoc, &TempRot, SpawnInfo));
	//NewPlayer->Possess(Cast<APawn>(TempActor));
	
}

void ASkyFlyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AllPlayerControllers.Add(Cast<ASkyFlyPlayerController>(NewPlayer));

	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("%d = Num"), NewPlayer->GetUniqueID()));*/

	UpdatePlayerList();
}

void ASkyFlyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPlayerControllers.Remove(Cast<ASkyFlyPlayerController>(Exiting));

	UpdatePlayerList();
}

void ASkyFlyGameModeBase::KickPlayer(uint8 PlayerID)
{
	AllPlayerControllers[PlayerID]->Client_KickPlayer();
}

void ASkyFlyGameModeBase::UpdatePlayerList()
{	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		GetGameState<ASkyFlyGameStateBase>()->UpdatePlayerList();
	}, 0.1, false);
	
}
