// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.h"
#include "SpawnBreakables.generated.h"

UCLASS()
class CHAOSTRUCK_API ASpawnBreakables : public AActor
{
	GENERATED_BODY()
	
private:
	//Minimum Distance that a breakable needs to be from others
	int minDistance = 150;
	int maxSpawnAttempts = 10;

public:	
	// Sets default values for this actor's properties
	ASpawnBreakables();

	UFUNCTION(BlueprintCallable)
	ASpawnArea* getSpawnObject();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AActor*> Destrutibles;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	FVector createSpawnLocation(int tries);
	// UFUNCTION(BlueprintCallable)
	// void AddSquare(FVector pos, int length, int width);
	// UFUNCTION(BlueprintCallable)
	// void AddCircle(FVector pos, int radius);
	// UFUNCTION(BlueprintCallable)
	// void AddDonut(FVector pos, int outerRadius, int innerRadius);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ASpawnArea*> spawnAreas;

	// UPROPERTY(EditAnywhere)
	// TArray<class AActor*> spawnShapes;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> getChildren();

};

// class Location {
// protected:
// 	FVector position;
// public:
// 	Location(FVector pos) {
// 		position = pos;
// 	}
// 	virtual FVector randomPos()
// 	{
// 		return FVector(0, 50, 0);
// 	}
// };
//
// class Circle : public Location {
// protected:
// 	int radius;
// public:
// 	Circle(FVector pos, int radiusIn) : Location(pos) {
// 		radius = radiusIn;
// 	}
//
// 	virtual FVector randomPos()
// 	{
// 		float degree = FMath::RandRange(0, 359);
// 		float distance = FMath::RandRange(0, radius);
//
// 		FVector spawnLocation = FVector(0, distance, 0).RotateAngleAxis(degree, FVector(0, 0, 1));
// 		spawnLocation += position;
// 		return spawnLocation;
// 	}
// };
//
// class Square : public Location {
// protected:
// 	int length;
// 	int width;
// public:
// 	Square(FVector pos, int lengthIn, int widthIn) : Location(pos) {
// 		length = lengthIn;
// 		width = widthIn;
// 	}
// 	virtual FVector randomPos()
// 	{
// 		float randLength = FMath::RandRange(0, length) - length/2;
// 		float randWidth = FMath::RandRange(0, width) - width/2;
//
// 		FVector spawnLocation = FVector(randLength, randWidth, 0) + position;
// 		return spawnLocation;
// 	}
// };
//
// class Donut : public Circle {
// protected:
// 	int innerRadius;
// public:
// 	Donut(FVector pos, int radiusOuter, int radiusInner) : Circle(pos,radiusOuter) {
// 		innerRadius = radiusInner;
// 	}
//
// 	virtual FVector randomPos()
// 	{
// 		float degree = FMath::RandRange(0, 359);
// 		float distance = FMath::RandRange(innerRadius, radius);
//
// 		FVector spawnLocation = FVector(0, distance, 0).RotateAngleAxis(degree, FVector(0, 0, 1));
// 		spawnLocation += position;
// 		return spawnLocation;
// 	}
// };
