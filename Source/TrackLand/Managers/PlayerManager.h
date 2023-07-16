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
	int WagonsObtained = 0;
	int Money = 9999;
	int LicenseLevel = 0;
	int TrainFuel = 999;

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
	int GetMoney() { return Money; }
	UFUNCTION(BlueprintCallable)
	int GetWagonsObtained() { return WagonsObtained; }
	UFUNCTION(BlueprintCallable)
	int GetLicenseLevel() { return LicenseLevel; }
};
