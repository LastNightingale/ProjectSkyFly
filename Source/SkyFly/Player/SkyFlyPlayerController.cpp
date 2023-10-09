// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyFlyPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ASkyFlyPlayerController::Client_KickPlayer_Implementation()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Minimal_Default");
}
