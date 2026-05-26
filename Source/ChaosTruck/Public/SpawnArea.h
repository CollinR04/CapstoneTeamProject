// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.generated.h"

UCLASS()
class CHAOSTRUCK_API ASpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnArea();

	UFUNCTION(BlueprintCallable)
	virtual FVector randomPos(int tries = 0);
	UFUNCTION(BlueprintCallable)
	virtual UClass* randomBreakable();

	UPROPERTY(EditAnywhere, Category = "Actor References")
	TArray<class UClass*> spawnableObjects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* spawner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int maxSpawnAttempts = 10;

	int minDistance = 2000;

	int distanceFromFloor = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
};
