#pragma once

#include "CoreMinimal.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "Manager.h"
#include "PlayerManager.generated.h"

UCLASS()
class TRACKLAND_API APlayerManager : public AManager
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	ATrainPawn *PlayerRef;
	int WagonsObtained = 1;
	int Money = 0;
	int LicenseLevel = 1;
	int TrainFuel = 100;
	AStation *CurrentStation = nullptr;

public:
	virtual void Tick(float DeltaTime) override;

	void UpgradeLicenseLevel();
	void UpgradeWagons();
	void AddMoney(int MoneyToAdd);
	void ConsumeFuel();
	void SubstractMoney(int MoneyToSubstract);

	UFUNCTION(BlueprintCallable)
	void SetTrainFuel(int Fuel) { TrainFuel = Fuel; }
	UFUNCTION(BlueprintCallable)
	int GetTrainFuel() { return TrainFuel; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentStation(AStation *NewStation) { CurrentStation = NewStation; }
	UFUNCTION(BlueprintCallable)
	AStation *GetCurrentStation() { return CurrentStation; }
};
