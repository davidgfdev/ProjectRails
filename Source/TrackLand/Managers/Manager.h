// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Manager.generated.h"

UCLASS()
class TRACKLAND_API AManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
