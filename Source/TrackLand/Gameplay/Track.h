// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Track.generated.h"

UCLASS()
class TRACKLAND_API ATrack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform &Transform) override;

	UPROPERTY(EditAnywhere)
	UStaticMesh *InstantiableMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DeformTrackMesh();
	void CreateCollisionInPoints();

	UPROPERTY(EditAnywhere)
	FVector ColliderSize = FVector(1,1,1);
};
