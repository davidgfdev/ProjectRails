// Fill out your copyright notice in the Description page of Project Settings.

#include "Track.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
ATrack::ATrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATrack::BeginPlay()
{
	Super::BeginPlay();

	USplineComponent *SplineRef = Cast<USplineComponent>(GetComponentByClass(USplineComponent::StaticClass()));

	CreateCollisionInPoint(0, SplineRef);
	CreateCollisionInPoint(SplineRef->GetNumberOfSplinePoints() - 1, SplineRef);
}

// Called every frame
void ATrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrack::OnConstruction(const FTransform &Transform)
{
	DeformTrackMesh();
}

void ATrack::DeformTrackMesh()
{
	USplineComponent *SplineRef = Cast<USplineComponent>(GetComponentByClass(USplineComponent::StaticClass()));

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
