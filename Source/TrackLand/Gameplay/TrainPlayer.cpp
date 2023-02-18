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
	SplineRef = FIRST_TRACK;
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
	else
	{
		UE_LOG(LogTemp, Display, TEXT("SPLINEREF IS NULL"));
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
	int SplineLength = SplineComponent->GetNumberOfSplinePoints();

	if (InverseSpline)
	{
		Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(SplineLength - 1);
		InverseSpline = false;
	}

	Distance = (DeltaTime * Speed) + Distance;

	FTransform TransformAtSpline = SplineComponent->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FTransform NewTransform = FTransform(TransformAtSpline.Rotator(), TransformAtSpline.GetLocation(), FVector::One());
	SetActorRelativeTransform(NewTransform);
}

void ATrainPlayer::SwitchToNewTrack(AActor *Track, bool IsBackwards)
{
	if (SplineRef != nullptr)
	{
		if (Track == SplineRef)
		{
			return;
		}

		USplineComponent *SplineComponent = Cast<USplineComponent>(SplineRef->GetComponentByClass(USplineComponent::StaticClass()));

		if (IsBackwards)
		{
			FVector FirstPoint = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
			double DistanceToPoint = FVector::Distance(FirstPoint, GetActorLocation());

			if (DistanceToPoint < 300 && GearIndex == 0)
			{
				SplineRef = Track;
				InverseSpline = true;
			}
		}
		else
		{
			FVector LastPoint = SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World);
			double DistanceToPoint = FVector::Distance(LastPoint, GetActorLocation());

			if (DistanceToPoint < 300 && GearIndex > 0)
			{
				SplineRef = Track;
				Distance = 0;
			}
		}
	}
}

int ATrainPlayer::GetGearIndex()
{
	return GearIndex;
}

int ATrainPlayer::GetDirection()
{
	return Direction;
}
