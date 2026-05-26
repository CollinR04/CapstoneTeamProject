// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnArea.h"

// Sets default values
ASpawnArea::ASpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector ASpawnArea::randomPos(int tries)
{
	return FVector(0,0,0);
}

UClass* ASpawnArea::randomBreakable()
{
	int rdm = FMath::RandRange(0, spawnableObjects.Num() - 1);
	return spawnableObjects[rdm];
}



// Called when the game starts or when spawned
void ASpawnArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

