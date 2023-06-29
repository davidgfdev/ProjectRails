#include "..\Source\TrackLand\Gameplay\Stations\Station.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Managers\PlayerManager.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Kismet/GameplayStatics.h"

AStation::AStation()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStation::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor *> AttachedActors;
	GetAttachedActors(AttachedActors);
	ChildTrack = Cast<ATrack>(AttachedActors[0]);
}

void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef != nullptr)
	{
		if (PlayerRef->CurrentTrack == ChildTrack && PlayerRef->GearIndex == 1 && IsAbleToStop)
		{
			OnTrainStopped();
			APlayerManager *PlayerManager = Cast<APlayerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerManager::StaticClass()));
			PlayerManager->SetCurrentStation(this);
			IsAbleToStop = false;
		}
		if (PlayerRef->CurrentTrack != ChildTrack)
		{
			IsAbleToStop = true;
		}
	}
}
