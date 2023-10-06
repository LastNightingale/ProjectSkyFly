// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerListItem.h"

#include "SkyFlyGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"

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
		KickButton->SetIsEnabled(false);		
		KickButton->SetToolTipText(FText::FromString("You can't kick yourself"));
	}

	KickButton->OnClicked.AddDynamic(this, &UPlayerListItem::OnClick);
}

void UPlayerListItem::OnClick()
{
	GetWorld()->GetAuthGameMode<ASkyFlyGameModeBase>()->KickPlayer(PlayerID);
}
