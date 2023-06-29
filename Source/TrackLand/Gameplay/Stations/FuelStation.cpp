// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "..\Source\TrackLand\Gameplay\Stations\FuelStation.h"

float AFuelStation::CalcFuelPrice(int CurrentFuel)
{
    int NeededFuel = 100 - CurrentFuel;
    float Price = NeededFuel * FuelPrice;
    return Price;
}

void AFuelStation::RefillTrain(APlayerManager *PlayerManager)
{
    float Price = CalcFuelPrice(PlayerManager->GetTrainFuel());
    PlayerManager->SubstractMoney(Price);
    PlayerManager->SetTrainFuel(100);
}
