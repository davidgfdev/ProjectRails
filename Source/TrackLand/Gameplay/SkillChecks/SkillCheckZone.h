#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "SkillCheckZone.generated.h"

UCLASS()
class TRACKLAND_API ASkillCheckZone : public AActor
{
	GENERATED_BODY()

public:
	ASkillCheckZone();

	UPROPERTY(EditDefaultsOnly)
	USceneComponent *Root;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent *Trigger;

protected:
	virtual void BeginPlay() override;
	void FailSkillCheck();

	UFUNCTION(BlueprintImplementableEvent)
	bool Check();

	UFUNCTION()
	void OnOverlapBegin(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex);
};
