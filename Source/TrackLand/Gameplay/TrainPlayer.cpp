// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Track.h"

// Sets default values
ATrainPlayer::ATrainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATrainPlayer::BeginPlay()
{
	Super::BeginPlay();

	AActor *FirstTrack = GetTrackInRange(500);
	if (FirstTrack != nullptr)
	{
		SplineRef = FirstTrack;
	}
}

// Called every frame
void ATrainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SplineRef != nullptr)
	{
		MoveObjectAlongSpline(DeltaTime);

		if (Speed != TargetSpeed)
		{
			Speed = FMath::Lerp(Speed, TargetSpeed, DeltaTime * FMath::Pow(ACCELERATION_RATE, 2));
		}
	}
}

// Called to bind functionality to input
void ATrainPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Accelerate", IE_Pressed, this, &ATrainPlayer::AcceleratePressed);
	PlayerInputComponent->BindAction("Slow", IE_Pressed, this, &ATrainPlayer::SlowPressed);
	PlayerInputComponent->BindAction("SwitchRight", IE_Pressed, this, &ATrainPlayer::SwitchRight);
	PlayerInputComponent->BindAction("SwitchLeft", IE_Pressed, this, &ATrainPlayer::SwitchLeft);
}

void ATrainPlayer::AcceleratePressed()
{
	if (SPEEDS.Num() == 4)
	{
		UE_LOG(LogTemp, Display, TEXT("SPEEDS ARE NOT SET!"));
		return;
	}

	if (GearIndex != 4)
	{
		GearIndex++;
	}

	TargetSpeed = SPEEDS[GearIndex];
}

void ATrainPlayer::SlowPressed()
{
	if (SPEEDS.Num() == 4)
	{
		UE_LOG(LogTemp, Display, TEXT("SPEEDS ARE NOT SET!"));
		return;
	}

	if (GearIndex != 0)
	{
		GearIndex--;
	}

	TargetSpeed = SPEEDS[GearIndex];
}

void ATrainPlayer::SwitchRight()
{
	Direction = 1;
}

void ATrainPlayer::SwitchLeft()
{
	Direction = 0;
}

void ATrainPlayer::MoveObjectAlongSpline(float DeltaTime)
{
	USplineComponent *SplineComponent = Cast<USplineComponent>(SplineRef->GetComponentByClass(USplineComponent::StaticClass()));

	Distance = (DeltaTime * Speed) + Distance;

	FTransform TransformAtSpline;

	if (IsReversed)
	{
		float SplineTotalDistance = SplineComponent->GetSplineLength();
		UE_LOG(LogTemp, Display, TEXT("SplineLength: %f"), SplineTotalDistance);
		TransformAtSpline = SplineComponent->GetTransformAtDistanceAlongSpline((SplineTotalDistance - Distance), ESplineCoordinateSpace::World);
	}
	else
	{
		TransformAtSpline = SplineComponent->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	}

	FTransform NewTransform = FTransform(TransformAtSpline.Rotator(), TransformAtSpline.GetLocation(), FVector::One());
	SetActorRelativeTransform(NewTransform);
}

void ATrainPlayer::SwitchToNewTrack(AActor *Track, bool IsBackwards)
{
	if (SplineRef == nullptr || (SplineRef != nullptr && Track == SplineRef))
		return;

	USplineComponent *SplineComponent = Cast<USplineComponent>(SplineRef->GetComponentByClass(USplineComponent::StaticClass()));

	if ((IsBackwards && GearIndex == 0) || (!IsBackwards && GearIndex > 0))
	{

		USplineComponent *NewSplineComponent = Cast<USplineComponent>(Track->GetComponentByClass(USplineComponent::StaticClass()));
		int ClosestPointIndex = GetClosestSplinePoint(NewSplineComponent);
		IsReversed = ClosestPointIndex == NewSplineComponent->GetNumberOfSplinePoints() - 1;

		SplineRef = Track;
		Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(ClosestPointIndex);

		UE_LOG(LogTemp, Display, TEXT("Distance: %f, ClosestPointIndex: %d"), Distance, ClosestPointIndex);
	}
}

int ATrainPlayer::GetClosestSplinePoint(USplineComponent *SplineComponent)
{
	int NumOfPoints = SplineComponent->GetNumberOfSplinePoints();
	double MinimalDistance = 999999;
	int MinimalPointIndex = 0;

	for (int i = 0; i <= NumOfPoints; i++)
	{
		FVector PointLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);

		double DistanceToThisPoint = FVector::Dist(GetActorLocation(), PointLocation);
		if (DistanceToThisPoint < MinimalDistance)
		{
			MinimalDistance = DistanceToThisPoint;
			MinimalPointIndex = i;
		}
	}

	return MinimalPointIndex;
}

AActor *ATrainPlayer::GetTrackInRange(float Radius = 300)
{
	TArray<AActor *> Tracks;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	TArray<AActor *> IgnoreActors;
	IgnoreActors.Init(this, 1);

	if (SplineRef != nullptr)
		IgnoreActors.Add(SplineRef);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, TraceObjectTypes, ATrack::StaticClass(), IgnoreActors, Tracks);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 10);

	return Tracks.Num() > 0
			   ? Tracks[0]
			   : nullptr;
}