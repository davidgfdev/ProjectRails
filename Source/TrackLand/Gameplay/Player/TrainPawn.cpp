#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Reverse.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Gameplay\Tracks\SwitchTrack.h"
#include "TrainPawn.h"

ATrainPawn::ATrainPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATrainPawn::BeginPlay()
{
	Super::BeginPlay();
	OverlapTracks();
}

void ATrainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TrackWaypoints.Num() != 0)
	{
		GearIndex == 0 ? MoveBackwards(DeltaTime) : MoveForward(DeltaTime);
	}

	if (CurrentSpeed != TargetSpeed)
	{
		CurrentSpeed = IsStucked ? 0 : FMath::Lerp(CurrentSpeed, TargetSpeed, DeltaTime * FMath::Pow(ACCELERATION_RATE, 2));
		// UE_LOG(LogTemp, Display, TEXT("CurrentSpeed: %f"), CurrentSpeed);
	}

	if (CurrentSpeed < 0.2 && GearIndex == 1)
	{
		CurrentSpeed = 0;
	}
}

void ATrainPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Accelerate", IE_Pressed, this, &ATrainPawn::AcceleratePressed);
	PlayerInputComponent->BindAction("Slow", IE_Pressed, this, &ATrainPawn::SlowPressed);
	PlayerInputComponent->BindAction("SwitchRight", IE_Pressed, this, &ATrainPawn::SwitchRight);
	PlayerInputComponent->BindAction("SwitchLeft", IE_Pressed, this, &ATrainPawn::SwitchLeft);
	PlayerInputComponent->BindAction("Whistle", IE_Pressed, this, &ATrainPawn::Whistle);
	PlayerInputComponent->BindAction("Whistle", IE_Released, this, &ATrainPawn::StopWhistle);
}

void ATrainPawn::AcceleratePressed()
{
	if (GearIndex != 4)
	{
		GearIndex++;
	}

	TargetSpeed = SPEEDS[GearIndex];
}

void ATrainPawn::SlowPressed()
{
	if (GearIndex != 0)
	{
		GearIndex--;
	}

	if (CurrentSpeed > 0.5 && (GearIndex == 1 || GearIndex == 0))
	{
		CurrentSpeed = 0;
		TargetSpeed = 0;
		IsStucked = true;
		FTimerHandle RegainControlHandle;
		GetWorldTimerManager().SetTimer(RegainControlHandle, this, &ATrainPawn::RecoverTrainControl, STUCK_TIME, false);
	}
	else
	{
		TargetSpeed = SPEEDS[GearIndex];
	}
}

void ATrainPawn::SwitchRight()
{
	PlayerDirection = 1;
}

void ATrainPawn::SwitchLeft()
{
	PlayerDirection = 0;
}

void ATrainPawn::Whistle()
{
	Whistling = true;
}

void ATrainPawn::StopWhistle()
{
	Whistling = false;
}

void ATrainPawn::OverlapTracks()
{
	TArray<AActor *> TracksFound;
	TArray<AActor *> SwitchesFound;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	TArray<AActor *> IgnoreActors;
	if (CurrentTrack != nullptr)
	{
		IgnoreActors.Init(CurrentTrack, 1);
	}

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 400, TraceObjectTypes, ATrack::StaticClass(), IgnoreActors, TracksFound);
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 400, TraceObjectTypes, ASwitchTrack::StaticClass(), IgnoreActors, SwitchesFound);

	TArray<AActor *> ArrayResult;
	ArrayResult.Append(TracksFound);
	ArrayResult.Append(SwitchesFound);

	float Distance = 200;
	AActor *Track = UGameplayStatics::FindNearestActor(GetActorLocation(), ArrayResult, Distance);

	if (Track != nullptr)
	{
		if (Track->IsA(ATrack::StaticClass()) && !Track->IsA(ASwitchTrack::StaticClass()))
		{
			ATrack *LinearTrack = Cast<ATrack>(Track);
			if (!LinearTrack->IsInMaintenance)
			{
				int ClosestPoint = LinearTrack->GetClosestPoint(LinearTrack->Spline, GetActorLocation());
				SwitchToNewTrack(LinearTrack, (ClosestPoint != 0));
			}
		}
		else if (Track->IsA(ASwitchTrack::StaticClass()))
		{
			ASwitchTrack *SwitchTrack = Cast<ASwitchTrack>(Track);
			int ClosestPoint = SwitchTrack->GetClosestPoint(SwitchTrack->ActiveSpline, GetActorLocation());
			SwitchToNewTrack(SwitchTrack, (ClosestPoint != 0));
		}
	}
}

void ATrainPawn::SwitchToNewTrack(AActor *Track, bool IsBackwards)
{
	if (Track->IsA(ATrack::StaticClass()) && !Track->IsA(ASwitchTrack::StaticClass()))
	{
		ATrack *LinearTrack = Cast<ATrack>(Track);
		LinearTrack->ToggleColliders(false);
		TrackWaypoints = LinearTrack->Waypoints;
	}
	else if (Track->IsA(ASwitchTrack::StaticClass()))
	{
		ASwitchTrack *SwitchTrack = Cast<ASwitchTrack>(Track);
		FName Tag = SwitchTrack->ActiveSpline->ComponentTags[0];
		UE_LOG(LogTemp, Display, TEXT("ActiveSpline TAG: %s"), *Tag.ToString());
		if (Tag.IsEqual("LinearSpline"))
		{
			TrackWaypoints = SwitchTrack->WaypointsLinear;
		}
		else if (Tag.IsEqual("RightSpline"))
		{
			TrackWaypoints = SwitchTrack->WaypointsRight;
		}
		else if (Tag.IsEqual("LeftSpline"))
		{
			TrackWaypoints = SwitchTrack->WaypointsLeft;
		}
		SwitchTrack->ToggleColliders(false);
	}

	if (GearIndex == 0)
	{
		if (!IsBackwards)
			Algo::Reverse(TrackWaypoints);
		WaypointIndex = TrackWaypoints.Num() - 1;
	}
	else
	{
		if (IsBackwards)
			Algo::Reverse(TrackWaypoints);
		WaypointIndex = 0;
	}

	if (CurrentTrack != nullptr)
	{
		CurrentTrack->IsA(ATrack::StaticClass()) ? Cast<ATrack>(CurrentTrack)->ToggleColliders(true) : Cast<ASwitchTrack>(CurrentTrack)->ToggleColliders(true);
	}

	CurrentTrack = Track;
}

void ATrainPawn::MoveForward(float DeltaSeconds)
{
	LookToNextWaypoint(false, DeltaSeconds);
	if (FVector::Distance(GetActorLocation(), TrackWaypoints[WaypointIndex]) <= POINT_RANGE)
	{
		(WaypointIndex >= TrackWaypoints.Num() - 1) ? OverlapTracks() : WaypointIndex++;
	}
	else
	{
		FVector WorldDirection = TrackWaypoints[WaypointIndex] - GetActorLocation();
		WorldDirection.Normalize();
		AddMovementInput(WorldDirection, CurrentSpeed, false);
	}
}

void ATrainPawn::MoveBackwards(float DeltaSeconds)
{
	LookToNextWaypoint(true, DeltaSeconds);
	if (FVector::Distance(GetActorLocation(), TrackWaypoints[WaypointIndex]) <= POINT_RANGE)
	{
		if (WaypointIndex <= 0)
		{
			WaypointIndex = 0;
			OverlapTracks();
		}
		else
		{
			WaypointIndex--;
		}
	}
	else
	{
		FVector WorldDirection = TrackWaypoints[WaypointIndex] - GetActorLocation();
		WorldDirection.Normalize();
		AddMovementInput(WorldDirection, CurrentSpeed, false);
	}
}

void ATrainPawn::LookToNextWaypoint(bool Inverse, float DeltaTime)
{
	UStaticMeshComponent *Locomotora = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (Locomotora != nullptr && TrackWaypoints.Num() != 0)
	{
		FRotator LookAt = Inverse
							  ? UKismetMathLibrary::FindLookAtRotation(TrackWaypoints[WaypointIndex], Locomotora->GetComponentLocation())
							  : UKismetMathLibrary::FindLookAtRotation(Locomotora->GetComponentLocation(), TrackWaypoints[WaypointIndex]);
		FRotator WorldRotation = UKismetMathLibrary::RInterpTo(Locomotora->GetComponentRotation(), LookAt, DeltaTime, 10);
		Locomotora->SetWorldRotation(WorldRotation);
	}
}

void ATrainPawn::RecoverTrainControl()
{
	IsStucked = false;
}
