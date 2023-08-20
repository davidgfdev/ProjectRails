#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Reverse.h"
#include "..\Source\TrackLand\Gameplay\Tracks\Track.h"
#include "..\Source\TrackLand\Gameplay\Tracks\SwitchTrack.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TrainComponent.h"
#include "TrainPawn.h"

ATrainPawn::ATrainPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	TrainComponent = CreateDefaultSubobject<UTrainComponent>("TrainComponent");
}

void ATrainPawn::BeginPlay()
{
	Super::BeginPlay();
	SkeletalMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	TrainComponent->Locomotora = SkeletalMesh;
}

void ATrainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsStucked)
	{
		if (CurrentSpeed != TargetSpeed)
		{
			CurrentSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, DeltaTime * ACCELERATION_RATE);
		}

		TrainComponent->CurrentSpeed = CurrentSpeed;
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
	TrainComponent->Inversor = false;
}

void ATrainPawn::SlowPressed()
{
	if (GearIndex != 0)
	{
		GearIndex--;
	}

	if (CurrentSpeed > 0.7 && (GearIndex == 1 || GearIndex == 0))
	{
		CurrentSpeed = 0;
		TargetSpeed = 0;
		TrainComponent->CurrentSpeed = 0;
		IsStucked = true;
		UAnimInstance *AnimInstance = SkeletalMesh->GetAnimInstance();
		AnimInstance->Montage_Play(StuckedMontage);
		FTimerHandle RegainControlHandle;
		GetWorldTimerManager().SetTimer(RegainControlHandle, this, &ATrainPawn::RecoverTrainControl, STUCK_TIME, false);
	}
	else
	{
		TargetSpeed = SPEEDS[GearIndex];
	}

	if (GearIndex == 0)
	{
		TrainComponent->Inversor = true;
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

void ATrainPawn::RecoverTrainControl()
{
	IsStucked = false;
	UAnimInstance *AnimInstance = SkeletalMesh->GetAnimInstance();
	AnimInstance->Montage_Stop(0);
}
