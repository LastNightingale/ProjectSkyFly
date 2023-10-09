// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerList.h"

#include "SkyFlyGameStateBase.h"
#include "Components/ScrollBox.h"
#include "GameFramework/PlayerState.h"

void UPlayerList::UpdateWidget()
{
	auto Players = GetWorld()->GetGameState<ASkyFlyGameStateBase>()->PlayerArray;
	

	PlayerListScrollBox->ClearChildren();

	for(uint8 Iter = 0; Iter < Players.Num(); ++Iter)
	{
		const auto CurrentWidget = Cast<UPlayerListItem>(CreateWidget(GetOwningPlayer(),ItemClass));
		CurrentWidget->Text = FText::FromString(Players[Iter]->GetPlayerName());
		CurrentWidget->PlayerID = Iter;
		PlayerListScrollBox->AddChild(CurrentWidget);
	}

	this->SetFocus();
}
