// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnArea.h"
#include "SpawnBreakables.h"
#include "SquareSpawnArea.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSTRUCK_API ASquareSpawnArea : public ASpawnArea
{
	GENERATED_BODY()
public:
	virtual FVector randomPos(int tries = 0) override;

};
