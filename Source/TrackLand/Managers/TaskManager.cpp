// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "Kismet/GameplayStatics.h"
#include "TaskManager.h"

void ATaskManager::AcceptMission(FMission Mission, int MissionIndex)
{
    if (CanAcceptMission(Mission))
    {
        IsMissionActive = true;
        CurrentMission = Mission;
        CurrentMissionIndex = MissionIndex;
        Satisfaction = 5;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Can't accept mission."));
    }
}

bool ATaskManager::CanAcceptMission(FMission Mission)
{
    APlayerManager *PlayerManager = Cast<APlayerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerManager::StaticClass()));

    int LicenseLevel = PlayerManager->GetLicenseLevel();
    int WagonLevel = PlayerManager->GetWagonsObtained();

    return (LicenseLevel >= Mission.LicenseLevel && WagonLevel >= Mission.WagonLevel && !IsMissionActive);
}

void ATaskManager::CompleteMission()
{
    APlayerManager *PlayerManager = Cast<APlayerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerManager::StaticClass()));
    if (Satisfaction >= CurrentMission.MinSatisfaction)
    {
        MissionsCompleted.Add(CurrentMissionIndex);
        PlayerManager->AddMoney(CurrentMission.Reward);
    }

    IsMissionActive = false;
    CurrentMission = FMission();
    CurrentMissionIndex = -1;
    Satisfaction = 5;
}