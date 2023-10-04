// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UICanvas.generated.h"

/**
 * 
 */
UCLASS()
class SKYFLY_API UICanvas : public UUserWidget
{
	GENERATED_BODY()
public:	

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* Canvas;	
};
