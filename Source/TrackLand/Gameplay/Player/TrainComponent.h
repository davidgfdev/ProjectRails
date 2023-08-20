// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TrainComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TRACKLAND_API UTrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTrainComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void OverlapTracks();
	void MoveForward(float DeltaSeconds);
	void MoveBackwards(float DeltaSeconds);
	void LookToNextWaypoint(float DeltaTime, bool Inverse = false);

	UFUNCTION(BlueprintCallable)
	void SwitchToNewTrack(AActor *Track, bool IsBackwards);

	USkeletalMeshComponent *Locomotora;
	AActor *CurrentTrack;
	float CurrentSpeed = 0;
	bool Inversor = false;

private:
	int WaypointIndex = 0;
	TArray<FVector> TrackWaypoints;

	UPROPERTY(EditAnywhere)
	float POINT_RANGE = 200;
};
