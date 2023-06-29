// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/StaticMesh.h"
#include "Track.generated.h"

UCLASS()
class TRACKLAND_API ATrack : public AActor
{
	GENERATED_BODY()

public:
	ATrack();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform &Transform) override;

	UPROPERTY(EditAnywhere)
	UStaticMesh *InstantiableMesh;

	UFUNCTION(BlueprintCallable)
	void DeformTrackMesh(USplineComponent *SplineRef);
	void CreateCollisionInPoint(int PositionIndex, USplineComponent *SplineRef);

	UPROPERTY(EditAnywhere)
	FVector ColliderSize = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere)
	bool IsAbleToMaintentance = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void ToggleColliders(bool IsActive);
	int GetClosestPoint(USplineComponent *SplineComponent, FVector Point);

	USplineComponent *Spline;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Waypoints;
	UPROPERTY(BlueprintReadOnly)
	bool IsInMaintenance = false;

	void TurnOnMaintenance();
};
