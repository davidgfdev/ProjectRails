// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "CoreMinimal.h"
#include "Station.h"
#include "LicenseStation.generated.h"

/**
 *
 */
UCLASS()
class TRACKLAND_API ALicenseStation : public AStation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<int> LicensePrices = {0, 2000, 3000, 4000, 5000, 6000};

	UFUNCTION(BlueprintCallable)
	int CalcLicenseUpgradePrice(APlayerManager *PlayerManager);

	UFUNCTION(BlueprintCallable)
	void UpgradeLicense(APlayerManager *PlayerManager);
};
