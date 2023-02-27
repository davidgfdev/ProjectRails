// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/SplineComponent.h"
#include "Track.h"
#include "TrainPlayer.generated.h"

UCLASS()
class TRACKLAND_API ATrainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Functions
	void AcceleratePressed();
	void SlowPressed();
	void SwitchRight();
	void SwitchLeft();

	// Functions
	void MoveObjectAlongSpline(float DeltaTime);
	void AdjustSpeedToGear();
	UFUNCTION(BlueprintCallable)
	void SwitchToNewTrack(AActor *Track, bool IsBackwards);
	int GetClosestSplinePoint(USplineComponent *SplineComponent);
	AActor* GetTrackInRange(float Radius);

	UPROPERTY(EditAnywhere)
	TArray<float> SPEEDS;
	UPROPERTY(EditAnywhere)
	float ACCELERATION_RATE;
	UPROPERTY(EditAnywhere)
	double DETECTION_DISTANCE = 300;

	float Distance = 0;
	AActor *SplineRef;
	float TargetSpeed = 0;
	bool InverseSpline = false;
	float Speed;
	bool IsReversed = false;

	/// 0 = Izquierda
	/// 1 = Derecha
	UPROPERTY(BlueprintReadOnly)
	int Direction = 0;
	/// 0 = Atrás
	/// 1 = Reposo
	/// 2 = Velocidad lenta
	/// 3 = Velocidad media
	/// 4 = Velocidad alta
	UPROPERTY(BlueprintReadOnly)
	int GearIndex = 1;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
