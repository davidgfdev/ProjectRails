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

	AActor *Track = FindSplineReference();
	if (Track != nullptr)
	{
		SplineRef = Track;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("COULDN'T FIND A VALID SPLINE"));
	}
}

// Called every frame
void ATrainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor *NewTrack = CheckForTracks();
	if (NewTrack != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("NewTrack: %s"), *NewTrack->GetActorNameOrLabel());
		SplineRef = NewTrack;
		Distance = 0;
	}

	if (SplineRef != nullptr)
	{
		MoveObjectAlongSpline(DeltaTime);

		if (Speed != TargetSpeed)
		{
			Speed = FMath::Lerp(Speed, TargetSpeed, DeltaTime * FMath::Pow(AccelerationRate, 2));
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
	if (Speeds.Num() == 4)
	{
		UE_LOG(LogTemp, Display, TEXT("SPEEDS ARE NOT SET!"));
		return;
	}

	if (GearIndex != 4)
	{
		GearIndex++;
	}

	TargetSpeed = Speeds[GearIndex];
}

void ATrainPlayer::SlowPressed()
{
	if (Speeds.Num() == 4)
	{
		UE_LOG(LogTemp, Display, TEXT("SPEEDS ARE NOT SET!"));
		return;
	}

	if (GearIndex != 0)
	{
		GearIndex--;
	}

	TargetSpeed = Speeds[GearIndex];
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

AActor *ATrainPlayer::FindSplineReference()
{
	TArray<AActor *> Tracks;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	TArray<AActor *> IgnoreActors;
	IgnoreActors.Init(this, 1);

	if (SplineRef != nullptr)
		IgnoreActors.Add(SplineRef);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation() + (OverlapOffset * GetActorForwardVector()), OverlapRadius, TraceObjectTypes, ATrack::StaticClass(), IgnoreActors, Tracks);

	if (Tracks.Num() > 0)
	{
		return Tracks[0];
	}

	return nullptr;
}

AActor *ATrainPlayer::CheckForTracks()
{
	AActor *TrackFound = FindSplineReference();

	if (TrackFound != nullptr)
	{
		USplineComponent *SplineComponent = Cast<USplineComponent>(TrackFound->GetComponentByClass(USplineComponent::StaticClass()));
		FVector SplineDirection = SplineComponent->GetDirectionAtSplinePoint(0, ESplineCoordinateSpace::World);
		int SplineLength = SplineComponent->GetNumberOfSplinePoints();

		FVector FirstPoint = SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		FVector LastPoint = SplineComponent->GetLocationAtSplinePoint(SplineLength, ESplineCoordinateSpace::World);

		if (GetActorLocation() == FirstPoint || GetActorLocation() == LastPoint)
		{
			if (GetActorLocation() == FirstPoint && GearIndex == 0)
			{
				InverseSpline = true;
			}
		}
		else
		{
			int TrackDirection = SplineDirection.X > 0 ? 1 : 0;

			if (TrackDirection != Direction)
			{
				UE_LOG(LogTemp, Display, TEXT("DIRECTION NOT DESIRED"));
				return nullptr;
			}
		}
	}

	return TrackFound;
}

int ATrainPlayer::GetGearIndex()
{
	return GearIndex;
}

int ATrainPlayer::GetDirection()
{
	return Direction;
}
