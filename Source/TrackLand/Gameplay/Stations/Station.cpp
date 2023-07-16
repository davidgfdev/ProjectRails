#include "..\Source\TrackLand\Gameplay\Stations\Station.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "..\Source\TrackLand\Managers\TaskManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AStation::AStation()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStation::BeginPlay()
{
	Super::BeginPlay();
}

void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef != nullptr)
	{
		StopTrain();
	}
	else
	{
		PlayerRef = Cast<ATrainPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrainPawn::StaticClass()));
	}
}

void AStation::StopTrain()
{
	float DistanceToStation = FVector::Distance(PlayerRef->GetActorLocation(), GetActorLocation());
	if (DistanceToStation < DISTANCE_THRESHOLD && PlayerRef->GearIndex == 1 && IsAbleToStop)
	{
		CheckMissionCompletion();
		OnTrainStopped();
		IsAbleToStop = false;
	}
	if (DistanceToStation > DISTANCE_THRESHOLD)
	{
		IsAbleToStop = true;
	}
}

void AStation::CheckMissionCompletion()
{
	ATaskManager *TaskManager = Cast<ATaskManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATaskManager::StaticClass()));

	int DestinationIndex = TaskManager->CurrentMission.StationId;
	if (DestinationIndex == StationId)
	{
		TaskManager->CompleteMission();
	}
}
