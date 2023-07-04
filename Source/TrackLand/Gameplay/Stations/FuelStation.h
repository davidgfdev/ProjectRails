#pragma once

#include "CoreMinimal.h"
#include "..\Source\TrackLand\Gameplay\Stations\Station.h"
#include "FuelStation.generated.h"

UCLASS()
class TRACKLAND_API AFuelStation : public AStation
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float FuelPrice = 0.8;

	UFUNCTION(BlueprintCallable)
	float CalcFuelPrice(int CurrentFuel);

	UFUNCTION(BlueprintCallable)
	void RefillTrain(APlayerManager *PlayerManager);
};
