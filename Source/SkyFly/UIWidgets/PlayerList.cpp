// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerList.h"

#include "Instruments/SkyFlyGameStateBase.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"
#include "Instruments/ProjectDeveloperSettings.h"

void UPlayerList::UpdateWidget()
{
	auto Players = GetWorld()->GetGameState<ASkyFlyGameStateBase>()->PlayerArray;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("UpdatedOld")));
	

	PlayerListScrollBox->ClearChildren();

	for(uint8 Iter = 0; Iter < Players.Num(); ++Iter)
	{
		const auto CurrentWidget = Cast<UPlayerListItem>(CreateWidget(GetOwningPlayer(),ItemClass));
		UE_LOG(LogTemp, Warning, TEXT("Iter: %d, %s"), Iter, *Players[Iter]->GetPlayerName());
		CurrentWidget->Text = FText::FromString(Players[Iter]->GetPlayerName());
		CurrentWidget->PlayerID = Iter;
		PlayerListScrollBox->AddChild(CurrentWidget);
	}

	this->SetFocus();
}

void UPlayerList::NativeConstruct()
{
	Super::NativeConstruct();

	ProjectSettings = GetDefault<UProjectDeveloperSettings>();
}

void UPlayerList::OnPlayerListUpdate(TArray<APlayerState*> Players)
{
	
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		FString::Printf(TEXT("Updated Player List")));*/
	if(!ProjectSettings)
		ProjectSettings = GetDefault<UProjectDeveloperSettings>();

	//new
	auto StatePlayers = GetWorld()->GetGameState<ASkyFlyGameStateBase>()->AllPlayerNames;
	
	PlayerListScrollBox->ClearChildren();

	for(uint8 Iter = 0; Iter < StatePlayers.Num(); ++Iter)
	{
		const auto CurrentWidget = Cast<UPlayerListItem>(CreateWidget(GetOwningPlayer(),ItemClass));
		CurrentWidget->Text = FText::FromString(StatePlayers[Iter]);
		CurrentWidget->UsernameText->SetColorAndOpacity(FLinearColor(ProjectSettings->PlayerColors[Iter]));
		CurrentWidget->PlayerID = Iter;
		PlayerListScrollBox->AddChild(CurrentWidget);
	}
	//this->SetFocus();
}
