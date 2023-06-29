#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrainPawn.generated.h"

UCLASS()
class TRACKLAND_API ATrainPawn : public APawn
{
	GENERATED_BODY()

public:
	ATrainPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SwitchToNewTrack(AActor *Track, bool IsBackwards);

	UPROPERTY(BlueprintReadOnly)
	int GearIndex = 1;
	UPROPERTY(BlueprintReadOnly)
	int PlayerDirection = 0;
	UPROPERTY(BlueprintReadOnly)
	float CurrentSpeed = 0;
	UPROPERTY(BlueprintReadOnly)
	bool Whistling;

	AActor *CurrentTrack;

private:
	void OverlapTracks();
	void MoveForward();
	void MoveBackwards();

	void AcceleratePressed();
	void SlowPressed();
	void SwitchRight();
	void SwitchLeft();
	void Whistle();
	void StopWhistle();

	void MoveForward(float DeltaSeconds);
	void MoveBackwards(float DeltaSeconds);

	void LookToNextWaypoint(bool Inverse, float DeltaTime);
	void RecoverTrainControl();

	int WaypointIndex = 0;

	TArray<FVector> TrackWaypoints;
	bool IsStucked = false;
	float TargetSpeed = 0;

	UPROPERTY(EditAnywhere)
	float ACCELERATION_RATE = 0.2;
	UPROPERTY(EditAnywhere)
	float POINT_RANGE = 200;
	UPROPERTY(EditAnywhere)
	TArray<float> SPEEDS = {20, 0, 20, 40, 60};
	UPROPERTY(EditAnywhere)
	double STUCK_TIME = 1.5;
};
