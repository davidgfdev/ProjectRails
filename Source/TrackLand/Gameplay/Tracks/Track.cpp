// Fill out your copyright notice in the Description page of Project Settings.

#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

ATrack::ATrack()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrack::BeginPlay()
{
	Super::BeginPlay();
	Spline = Cast<USplineComponent>(GetComponentByClass(USplineComponent::StaticClass()));
	TurnOnMaintenance();
}

void ATrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrack::OnConstruction(const FTransform &Transform)
{
	USplineComponent *SplineRef = Cast<USplineComponent>(GetComponentByClass(USplineComponent::StaticClass()));
	DeformTrackMesh(SplineRef);
}

void ATrack::DeformTrackMesh(USplineComponent *SplineRef)
{
	if (InstantiableMesh == nullptr || SplineRef == nullptr)
	{
		return;
	}

	int SplinePoints = SplineRef->GetNumberOfSplinePoints() - 2;

	for (int i = 0; i <= SplinePoints; i++)
	{
		FVector StartLocation = FVector(0, 0, 0);
		FVector StartTangent = FVector(0, 0, 0);
		SplineRef->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::Local);

		FVector EndLocation = FVector(0, 0, 0);
		FVector EndTangent = FVector(0, 0, 0);
		SplineRef->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);

		USplineMeshComponent *SplineMesh = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), false, FTransform::Identity, false));
		SplineMesh->SetStaticMesh(InstantiableMesh);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::X);
		SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);

		SplineMesh->AttachToComponent(SplineRef, FAttachmentTransformRules::KeepRelativeTransform, TEXT("SplineMesh"));
	}
}

void ATrack::CreateCollisionInPoint(int PositionIndex, USplineComponent *SplineRef)
{
	FVector NewColliderPosition = SplineRef->GetLocationAtSplinePoint(PositionIndex, ESplineCoordinateSpace::Local);
	FTransform NewColliderTransform = FTransform(FRotator::ZeroRotator, NewColliderPosition, ColliderSize);
	UBoxComponent *BoxComponent = Cast<UBoxComponent>(AddComponentByClass(UBoxComponent::StaticClass(), false, NewColliderTransform, false));
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}

void ATrack::ToggleColliders(bool IsActive)
{
	TArray<UBoxComponent *> Components;
	GetComponents<UBoxComponent>(Components);

	for (UBoxComponent *BoxCollider : Components)
	{
		BoxCollider->SetGenerateOverlapEvents(IsActive);
	}
}

int ATrack::GetClosestPoint(USplineComponent *SplineComponent, FVector Point)
{
	float MinimalDistance = 9999;
	int MinimalPoint = 0;
	for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints(); i++)
	{
		FVector SplinePointLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		float Distance = FVector::Distance(Point, SplinePointLocation);

		if (Distance < MinimalDistance)
		{
			MinimalDistance = Distance;
			MinimalPoint = i;
		}
	}

	return MinimalPoint;
}

void ATrack::TurnOnMaintenance()
{
	if (IsAbleToMaintentance)
	{
		IsInMaintenance = FMath::RandRange(0, 100) > 90;

		TArray<UActorComponent *> ActorsWithTag = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Indicator"));
		if (ActorsWithTag.Num() != 0)
		{
			UStaticMeshComponent *Indicator = Cast<UStaticMeshComponent>(ActorsWithTag[0]);
			if (Indicator != nullptr)
				Indicator->SetVisibility(IsInMaintenance);
		}
	}
}
