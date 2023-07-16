// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/StaticMesh.h"
#include "SwitchTrack.generated.h"

UENUM(BlueprintType)
enum EEntrances
{
	CENTER UMETA(DisplayName = "Center"),
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right"),
};

UCLASS()
class TRACKLAND_API ASwitchTrack : public ATrack
{
	GENERATED_BODY()

public:
	ASwitchTrack();

	USplineComponent *GetActiveSpline();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform &Transform) override;
	void DeformTrackMesh(USplineComponent *SplineRef);

	UFUNCTION(BlueprintCallable) void CalculateValidSpline(TEnumAsByte<EEntrances> Entrance, int PlayerDirection);

public:
UPROPERTY(BlueprintReadWrite) USplineComponent *ActiveSpline;
	UPROPERTY(BlueprintReadWrite) TArray<FVector> WaypointsLinear;
	UPROPERTY(BlueprintReadWrite) TArray<FVector> WaypointsLeft;
	UPROPERTY(BlueprintReadWrite) TArray<FVector> WaypointsRight;

	virtual void Tick(float DeltaTime) override;
	void ToggleColliders(bool IsActive);
};
