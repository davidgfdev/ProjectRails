// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "LicenseStation.h"

void ALicenseStation::UpgradeLicense(APlayerManager *PlayerManager)
{
    int UpgradePrice = CalcLicenseUpgradePrice(PlayerManager);
    int PlayerMoney = PlayerManager->GetMoney();
    if (PlayerMoney >= UpgradePrice)
    {
        PlayerManager->SubstractMoney(UpgradePrice);
        PlayerManager->UpgradeLicenseLevel();
    }
}

int ALicenseStation::CalcLicenseUpgradePrice(APlayerManager *PlayerManager)
{
    int CurrentLicenseTier = PlayerManager->GetLicenseLevel();
    int Price = LicensePrices[CurrentLicenseTier + 1];
    return Price;
}
