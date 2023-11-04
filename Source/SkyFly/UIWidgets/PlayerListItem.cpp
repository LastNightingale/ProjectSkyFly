// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListItem.h"
#include "Instruments/SkyFlyGameModeBase.h"
#include "Components/Button.h"
#include "Instruments/LobbyGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UPlayerListItem::NativeConstruct()
{
	Super::NativeConstruct();

	UsernameText->SetText(Text);

	if(UKismetSystemLibrary::IsServer(GetWorld()))
	{
		KickButton->SetVisibility(ESlateVisibility::Visible);
	}

	if(PlayerID == 0)
	{
		KickButton->SetVisibility(ESlateVisibility::Hidden);
	}

	KickButton->OnClicked.AddDynamic(this, &UPlayerListItem::OnClick);
}

void UPlayerListItem::OnClick()
{
	if(auto SGM = GetWorld()->GetAuthGameMode<ASkyFlyGameModeBase>())
	{
		SGM->KickPlayer(PlayerID);
	}		
	else if (auto LGM =GetWorld()->GetAuthGameMode<ALobbyGameModeBase>())
	{
		LGM->KickPlayer(PlayerID);
	}
}
