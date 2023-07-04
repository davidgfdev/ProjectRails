#include "..\Source\TrackLand\Gameplay\Stations\Station.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Managers\PlayerManager.h"
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
		float DistanceToStation = FVector::Distance(PlayerRef->GetActorLocation(), GetActorLocation());
		if (DistanceToStation < DISTANCE_THRESHOLD && PlayerRef->GearIndex == 1 && IsAbleToStop)
		{
			OnTrainStopped();
			APlayerManager *PlayerManager = Cast<APlayerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerManager::StaticClass()));
			IsAbleToStop = false;
		}
		if (DistanceToStation > DISTANCE_THRESHOLD)
		{
			IsAbleToStop = true;
		}
	}
	else
	{
		PlayerRef = Cast<ATrainPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrainPawn::StaticClass()));
	}
}
