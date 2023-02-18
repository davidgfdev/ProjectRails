// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
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

	// Getters
	UFUNCTION(BlueprintCallable)
	int GetGearIndex();
	UFUNCTION(BlueprintCallable)
	int GetDirection();

	UPROPERTY(EditAnywhere)
	TArray<float> SPEEDS;
	UPROPERTY(EditAnywhere)
	float ACCELERATION_RATE;
	UPROPERTY(BlueprintReadWrite)
	AActor *FIRST_TRACK;

	/// 0 = Atrás
	/// 1 = Reposo
	/// 2 = Velocidad lenta
	/// 3 = Velocidad media
	/// 4 = Velocidad alta
	int GearIndex = 1;
	AActor *SplineRef;
	float Distance = 0;
	float TargetSpeed = 0;
	bool InverseSpline = false;
	float Speed;
	/// 0 = Izquierda
	/// 1 = Derecha
	int Direction = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
