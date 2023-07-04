// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Source\TrackLand\Gameplay\Stations\Station.h"
#include "WorkshopStation.h"

void AWorkshopStation::UpgradeWagons(APlayerManager *PlayerManager)
{
    int PlayerMoney = PlayerManager->GetMoney();
    int NextWagonPrice = CalcUpgradePrice(PlayerManager);
    if (PlayerMoney >= NextWagonPrice)
    {
        PlayerManager->UpgradeWagons();
    }
}

int AWorkshopStation::CalcUpgradePrice(APlayerManager *PlayerManager)
{
    return WagonPrices[PlayerManager->GetWagonsObtained() + 1];
}
