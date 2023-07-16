#pragma once

#include "CoreMinimal.h"
#include "..\Source\TrackLand\Gameplay\Missions\MissionsDB.h"
#include "Station.h"
#include "MissionStation.generated.h"

UCLASS()
class TRACKLAND_API AMissionStation : public AStation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int> StationMissionsIndex;
};