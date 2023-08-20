// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainComponent.h"
#include "TransitTrain.generated.h"

UCLASS()
class TRACKLAND_API ATransitTrain : public APawn
{
	GENERATED_BODY()

public:
	ATransitTrain();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	int GearIndex = 1;
	UPROPERTY(BlueprintReadOnly)
	int CurrentDirection = 0;
	UPROPERTY(BlueprintReadOnly)
	float Speed = 20;

	AActor *CurrentTrack;
	UTrainComponent* TrainComponent;

protected:
	virtual void BeginPlay() override;
};
