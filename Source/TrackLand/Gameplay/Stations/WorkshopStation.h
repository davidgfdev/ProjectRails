// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Station.h"
#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "WorkshopStation.generated.h"

/**
 *
 */
UCLASS()
class TRACKLAND_API AWorkshopStation : public AStation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<int> WagonPrices = {500, 750, 1200, 1800, 3000};

	UFUNCTION(BlueprintCallable)
	void UpgradeWagons(APlayerManager *PlayerManager);

	UFUNCTION(BlueprintCallable)
	int CalcUpgradePrice(APlayerManager *PlayerManager);
};
