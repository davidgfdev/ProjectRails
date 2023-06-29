#include "Manager.h"

AManager::AManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AManager::BeginPlay()
{
	Super::BeginPlay();
}

void AManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

