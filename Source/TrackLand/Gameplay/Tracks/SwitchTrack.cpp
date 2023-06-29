#include "..\Source\TrackLand\Gameplay\Tracks\SwitchTrack.h"

ASwitchTrack::ASwitchTrack()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASwitchTrack::BeginPlay()
{
    Super::BeginPlay();
}

void ASwitchTrack::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASwitchTrack::DeformTrackMesh(USplineComponent *SplineRef)
{
    Super::DeformTrackMesh(SplineRef);
}

void ASwitchTrack::OnConstruction(const FTransform &Transform)
{
    Super::OnConstruction(Transform);

    TArray<UActorComponent *> SplineComponents = GetComponentsByClass(USplineComponent::StaticClass());
    for (UActorComponent *SplineComponent : SplineComponents)
    {
        USplineComponent *SplineRef = Cast<USplineComponent>(SplineComponent);
        DeformTrackMesh(SplineRef);
    }
}

void ASwitchTrack::CalculateValidSpline(TEnumAsByte<EEntrances> Entrance, int PlayerDirection)
{
    UE_LOG(LogTemp, Display, TEXT("Calculando spline valida"));
    UActorComponent *SplineRef = nullptr;
    FString Tag = "";
    switch (Entrance)
    {
    case EEntrances::CENTER:
        Tag = PlayerDirection == 1 ? "RightSpline" : "LeftSpline";
        break;

    case EEntrances::LEFT:
        Tag = PlayerDirection == 1 ? "LeftSpline" : "LinearSpline";
        break;

    case EEntrances::RIGHT:
        Tag = PlayerDirection == 1 ? "LinearSpline" : "RightSpline";
        break;
    }

    ActiveSpline = Cast<USplineComponent>(GetComponentsByTag(USplineComponent::StaticClass(), FName(Tag))[0]);
}

USplineComponent *ASwitchTrack::GetActiveSpline()
{
    return ActiveSpline;
}

void ASwitchTrack::ToggleColliders(bool IsActive)
{
    Super::ToggleColliders(IsActive);
}
