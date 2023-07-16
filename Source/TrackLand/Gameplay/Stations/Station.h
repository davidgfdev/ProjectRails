#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "Station.generated.h"

UCLASS()
class TRACKLAND_API AStation : public AActor
{
	GENERATED_BODY()

public:
	AStation();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTrainStopped();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StationId = -1;
	UPROPERTY(BlueprintReadWrite)
	ATrainPawn *PlayerRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString StationName = "";
	UPROPERTY(EditAnywhere)
	float DISTANCE_THRESHOLD = 500;

protected:
	virtual void BeginPlay() override;

private:
	bool IsAbleToStop = true;

	void StopTrain();
	void CheckMissionCompletion();
};
