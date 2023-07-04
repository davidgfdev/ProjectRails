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

	UPROPERTY(BlueprintReadWrite)
	ATrainPawn *PlayerRef;

	UPROPERTY(EditAnywhere)
	float DISTANCE_THRESHOLD = 500;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTrainStopped();

protected:
	virtual void BeginPlay() override;

private:
	bool IsAbleToStop = true;

public:
	virtual void Tick(float DeltaTime) override;
};
