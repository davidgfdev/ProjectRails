#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wagon.generated.h"

UCLASS()
class TRACKLAND_API AWagon : public AActor
{
	GENERATED_BODY()

public:
	AWagon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor *NextWagon;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void FollowNextWagon(float DeltaTime);
	void LookToNextWagon(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float SPEED = 1.0;
};
