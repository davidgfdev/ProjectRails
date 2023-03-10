// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
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

	SwitchToNewTrack(FindSplineReference(300), false);
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
			Speed = IsStucked ? 0 : FMath::Lerp(Speed, TargetSpeed, DeltaTime * FMath::Pow(ACCELERATION_RATE, 2));
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

	if (Speed > 750 && (GearIndex == 1 || GearIndex == 0))
	{
		TargetSpeed = 0;
		Speed = 0;
		IsStucked = true;
		FTimerHandle RegainControlHandle;
		GetWorldTimerManager().SetTimer(RegainControlHandle, this, &ATrainPlayer::RecoverTrainControl, STUCK_TIME, false);
	}
	else
	{
		TargetSpeed = SPEEDS[GearIndex];
	}
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
	int SplineLength = SplineComponent->GetNumberOfSplinePoints();

	Distance = (DeltaTime * Speed * FacingDirection) + Distance;

	FTransform TransformAtSpline = SplineComponent->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	FTransform NewTransform = FTransform(TransformAtSpline.Rotator(), TransformAtSpline.GetLocation(), FVector::One());

	if (FacingDirection == -1)
	{
		FVector SplineBackwardVector = -NewTransform.GetRotation().GetForwardVector();
		NewTransform = FTransform(SplineBackwardVector.Rotation(), TransformAtSpline.GetLocation(), FVector::One());
	}

	SetActorRelativeTransform(NewTransform);
}

void ATrainPlayer::SwitchToNewTrack(AActor *Track, bool IsBackwards)
{
	if (Track == SplineRef)
		return;
	USplineComponent *NewTrackSplineComponent = Cast<USplineComponent>(Track->GetComponentByClass(USplineComponent::StaticClass()));

	int ClosestPointIndex = GetClosestSplinePoint(NewTrackSplineComponent);

	SplineRef = Track;

	FacingDirection = (ClosestPointIndex == 0)
						  ? FacingDirection = (GearIndex > 0) ? 1 : -1
						  : FacingDirection = (GearIndex > 0) ? -1 : 1;

	Distance = ClosestPointIndex == 0 ? 0 : (NewTrackSplineComponent->GetSplineLength());
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

AActor *ATrainPlayer::FindSplineReference(float Radius)
{
	TArray<AActor *> Tracks;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	TArray<AActor *> IgnoreActors;
	IgnoreActors.Init(this, 1);

	if (SplineRef != nullptr)
		IgnoreActors.Add(SplineRef);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, TraceObjectTypes, ATrack::StaticClass(), IgnoreActors, Tracks);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 5);

	if (Tracks.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Track found"));
		return Tracks[0];
	}

	UE_LOG(LogTemp, Display, TEXT("Track NOT found"));
	return nullptr;
}

void ATrainPlayer::RecoverTrainControl()
{
	IsStucked = false;
	UE_LOG(LogTemp, Display, TEXT("Control recovered"));
}