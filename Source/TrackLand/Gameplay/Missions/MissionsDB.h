// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "..\Source\TrackLand\Gameplay\Stations\MissionStation.h"
#include "..\Source\TrackLand\Gameplay\Stations\Station.h"
#include "MissionsDB.generated.h"

USTRUCT(Blueprintable)
struct FMission
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MissionName = "DEFAULT MISSION";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StationId = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LicenseLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int WagonLevel = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Reward = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MinSatisfaction = 1;
};

UCLASS()
class TRACKLAND_API UMissionsDB : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMission> Missions;
};
