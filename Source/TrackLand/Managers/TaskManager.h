#pragma once

#include "CoreMinimal.h"
#include "..\Source\TrackLand\Gameplay\Missions\MissionsDB.h"
#include "Manager.h"
#include "TaskManager.generated.h"

UCLASS()
class TRACKLAND_API ATaskManager : public AManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMissionsDB *MissionDatabase;
	UPROPERTY(BlueprintReadOnly)
	int CurrentMissionIndex;
	UPROPERTY(BlueprintReadOnly)
	FMission CurrentMission;
	UPROPERTY(BlueprintReadOnly)
	int Satisfaction;
	UPROPERTY(BlueprintReadOnly)
	bool IsMissionActive = false;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> MissionsCompleted;

	UFUNCTION(BlueprintCallable)
	void AcceptMission(FMission Mission, int MissionIndex);

	void CompleteMission();

private:
	bool CanAcceptMission(FMission Mission);
};
