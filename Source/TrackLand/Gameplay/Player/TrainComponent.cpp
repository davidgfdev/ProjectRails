#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Reverse.h"
#include "TrainPawn.h"
#include "..\Source\TrackLand\Gameplay\AI\TransitTrain.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Gameplay\Tracks\SwitchTrack.h"
#include "Components/SceneComponent.h"
#include "TrainComponent.h"

UTrainComponent::UTrainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTrainComponent::BeginPlay()
{
	Super::BeginPlay();
	OverlapTracks();
}

void UTrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TrackWaypoints.Num() != 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Hay Waypoints"));
		Inversor ? MoveBackwards(DeltaTime) : MoveForward(DeltaTime);
	}
}

void UTrainComponent::OverlapTracks()
{
	TArray<AActor *> TracksFound;
	TArray<AActor *> SwitchesFound;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	FVector Location = GetOwner()->GetActorLocation();

	TArray<AActor *> IgnoreActors;
	if (CurrentTrack != nullptr)
	{
		IgnoreActors.Init(CurrentTrack, 1);
	}

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, 400, TraceObjectTypes, ATrack::StaticClass(), IgnoreActors, TracksFound);
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Location, 400, TraceObjectTypes, ASwitchTrack::StaticClass(), IgnoreActors, SwitchesFound);

	TArray<AActor *> ArrayResult;
	ArrayResult.Append(TracksFound);
	ArrayResult.Append(SwitchesFound);

	float Distance = 200;
	AActor *Track = UGameplayStatics::FindNearestActor(Location, ArrayResult, Distance);

	if (Track != nullptr)
	{
		if (Track->IsA(ATrack::StaticClass()) && !Track->IsA(ASwitchTrack::StaticClass()))
		{
			ATrack *LinearTrack = Cast<ATrack>(Track);
			if (!LinearTrack->IsBlocked)
			{
				int ClosestPoint = LinearTrack->GetClosestPoint(LinearTrack->Spline, Location);
				SwitchToNewTrack(LinearTrack, (ClosestPoint != 0));
			}
		}
		else if (Track->IsA(ASwitchTrack::StaticClass()))
		{
			ASwitchTrack *SwitchTrack = Cast<ASwitchTrack>(Track);
			int ClosestPoint = SwitchTrack->GetClosestPoint(SwitchTrack->ActiveSpline, Location);
			SwitchToNewTrack(SwitchTrack, (ClosestPoint != 0));
		}
	}
}

void UTrainComponent::SwitchToNewTrack(AActor *Track, bool IsBackwards)
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

	if (Inversor)
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

void UTrainComponent::MoveForward(float DeltaSeconds)
{
	UE_LOG(LogTemp, Display, TEXT("MOVING FORWARD"));
	FVector Location = GetOwner()->GetActorLocation();
	LookToNextWaypoint(DeltaSeconds, false);
	if (FVector::Distance(Location, TrackWaypoints[WaypointIndex]) <= POINT_RANGE)
	{
		(WaypointIndex >= TrackWaypoints.Num() - 1) ? OverlapTracks() : WaypointIndex++;
	}
	else
	{
		FVector WorldDirection = TrackWaypoints[WaypointIndex] - Location;
		WorldDirection.Normalize();
		if (GetOwner()->IsA(ATrainPawn::StaticClass()))
		{
			Cast<ATrainPawn>(GetOwner())->AddMovementInput(WorldDirection, CurrentSpeed, false);
		}
		else
		{
			Cast<ATransitTrain>(GetOwner())->AddMovementInput(WorldDirection, CurrentSpeed, false);
		}
	}
}

void UTrainComponent::MoveBackwards(float DeltaSeconds)
{
	FVector Location = GetOwner()->GetActorLocation();
	LookToNextWaypoint(DeltaSeconds, true);
	if (FVector::Distance(Location, TrackWaypoints[WaypointIndex]) <= POINT_RANGE)
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
		FVector WorldDirection = TrackWaypoints[WaypointIndex] - Location;
		WorldDirection.Normalize();
		Cast<ATrainPawn>(GetOwner())->AddMovementInput(WorldDirection, CurrentSpeed, false);
	}
}

void UTrainComponent::LookToNextWaypoint(float DeltaTime, bool Inverse)
{
	FVector Location = GetOwner()->GetActorLocation();
	if (Locomotora != nullptr && TrackWaypoints.Num() != 0)
	{
		AActor *LocomotoraActor = Locomotora->GetOwner();
		UE_LOG(LogTemp, Display, TEXT("GIRANDO"));
		FRotator LookAt = Inverse
							  ? UKismetMathLibrary::FindLookAtRotation(TrackWaypoints[WaypointIndex], LocomotoraActor->GetActorLocation())
							  : UKismetMathLibrary::FindLookAtRotation(LocomotoraActor->GetActorLocation(), TrackWaypoints[WaypointIndex]);
		FRotator WorldRotation = UKismetMathLibrary::RInterpTo(LocomotoraActor->GetActorRotation(), LookAt, DeltaTime, 10);
		LocomotoraActor->SetActorRotation(WorldRotation);
		// Locomotora->SetWorldRotation(WorldRotation);
	}
}
