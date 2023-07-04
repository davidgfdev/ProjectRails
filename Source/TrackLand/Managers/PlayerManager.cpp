// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerManager.h"

void APlayerManager::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerManager::SubstractMoney(int MoneyToSubstract)
{
    Money -= MoneyToSubstract;
}

void APlayerManager::UpgradeWagons()
{
    if (WagonsObtained < 5)
    {
        WagonsObtained++;
    }
}