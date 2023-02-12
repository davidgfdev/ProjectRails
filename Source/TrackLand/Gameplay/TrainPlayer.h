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

	//Functions
	AActor* FindSplineReference();
	void MoveObjectAlongSpline(float DeltaTime);
	void AdjustSpeedToGear();
	AActor* CheckForTracks();

	//Getters
	UFUNCTION(BlueprintCallable)
	int GetGearIndex();
	UFUNCTION(BlueprintCallable)
	int GetDirection();

	UPROPERTY(EditAnywhere)
	float OverlapRadius;
	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(EditAnywhere)
	TArray<float> Speeds;
	UPROPERTY(EditAnywhere)
	float AccelerationRate;
	UPROPERTY(EditAnywhere)
	FVector OverlapOffset;

	AActor *SplineRef;

	/// 0 = Atrás
	/// 1 = Reposo
	/// 2 = Velocidad lenta
	/// 3 = Velocidad media
	/// 4 = Velocidad alta
	int GearIndex = 1;
	float Distance = 0;
	float TargetSpeed = 0;
	bool InverseSpline = false;
	/// 0 = Izquierda
	/// 1 = Derecha
	int Direction = 0; 

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
