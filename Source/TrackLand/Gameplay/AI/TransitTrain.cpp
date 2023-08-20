#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainComponent.h"
#include "..\Source\TrackLand\Gameplay\Tracks\SwitchTrack.h"
#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "TransitTrain.h"

ATransitTrain::ATransitTrain()
{
	PrimaryActorTick.bCanEverTick = true;
	TrainComponent = CreateDefaultSubobject<UTrainComponent>("TrainComponent");
}

void ATransitTrain::BeginPlay()
{
	Super::BeginPlay();
	TrainComponent->Locomotora = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	TrainComponent->CurrentSpeed = 20;
}

void ATransitTrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
