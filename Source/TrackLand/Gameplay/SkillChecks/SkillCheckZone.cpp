#include "..\Source\TrackLand\Gameplay\Player\TrainPawn.h"
#include "Components/BoxComponent.h"
#include "SkillCheckZone.h"

ASkillCheckZone::ASkillCheckZone()
{
    Root = CreateDefaultSubobject<USceneComponent>("Root");
    Root->SetupAttachment(GetRootComponent());
    Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
    Trigger->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASkillCheckZone::FailSkillCheck()
{
    UE_LOG(LogTemp, Display, TEXT("SKILL CHECK FALLIDA"));
}

void ASkillCheckZone::BeginPlay()
{
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASkillCheckZone::OnOverlapBegin);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ASkillCheckZone::OnOverlapEnd);
}

void ASkillCheckZone::OnOverlapBegin(
    class UPrimitiveComponent *OverlappedComp,
    class AActor *OtherActor,
    class UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult &SweepResult)
{
    UE_LOG(LogTemp, Display, TEXT("OverlapBegin"));
    if (OtherActor->IsA(ATrainPawn::StaticClass()))
    {
        if (Check())
            FailSkillCheck();
    }
}

void ASkillCheckZone::OnOverlapEnd(
    class UPrimitiveComponent *OverlappedComp,
    class AActor *OtherActor,
    class UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Display, TEXT("OverlapBegin"));
    if (OtherActor->IsA(ATrainPawn::StaticClass()))
    {
        if (Check())
            FailSkillCheck();
    }
}
