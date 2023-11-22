// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerList.h"

#include "Instruments/SkyFlyGameStateBase.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"
#include "Instruments/ProjectDeveloperSettings.h"

void UPlayerList::NativeConstruct()
{
	Super::NativeConstruct();

	ProjectSettings = GetDefault<UProjectDeveloperSettings>();
}

void UPlayerList::OnPlayerListUpdate(TArray<APlayerState*> Players)
{
	if(!ProjectSettings)
		ProjectSettings = GetDefault<UProjectDeveloperSettings>();
	
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
}
