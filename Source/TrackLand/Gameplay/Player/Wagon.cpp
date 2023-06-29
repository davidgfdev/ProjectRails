#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Wagon.h"

AWagon::AWagon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWagon::BeginPlay()
{
	Super::BeginPlay();
}

void AWagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowNextWagon(DeltaTime);
}

void AWagon::FollowNextWagon(float DeltaTime)
{
	if (NextWagon != nullptr)
	{
		LookToNextWagon(DeltaTime);
		UChildActorComponent *WagonAttach = Cast<UChildActorComponent>(NextWagon->GetComponentsByTag(UChildActorComponent::StaticClass(), FName(TEXT("WagonAttach")))[0]);
		SetActorLocation(FMath::Lerp(GetActorLocation(), WagonAttach->GetComponentLocation(), DeltaTime * SPEED));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No tiene NextWagon asignado."));
	}
}

void AWagon::LookToNextWagon(float DeltaTime)
{
	UStaticMeshComponent *Wagon = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	UChildActorComponent *NextWagonAttach = Cast<UChildActorComponent>(NextWagon->GetComponentsByTag(UChildActorComponent::StaticClass(), FName(TEXT("WagonAttach")))[0]);
	if (Wagon != nullptr)
	{
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Wagon->GetComponentLocation(), NextWagonAttach->GetComponentLocation());
		FRotator WorldRotation = UKismetMathLibrary::RInterpTo(Wagon->GetComponentRotation(), LookAt, DeltaTime, 10);
		Wagon->SetWorldRotation(WorldRotation);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("El StaticMesh del vagón no existe."));
	}
}
