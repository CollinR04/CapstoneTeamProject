// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBreakables.h"
#include "Breakable.h"

// Sets default values
ASpawnBreakables::ASpawnBreakables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TArray<class AActor*> ASpawnBreakables::getChildren()
{
	return Destrutibles;
}

// Called when the game starts or when spawned
void ASpawnBreakables::BeginPlay()
{
	Destrutibles = {};
	TArray<class USceneComponent*> children;
	RootComponent->GetChildrenComponents(false, children);
	for (USceneComponent* child : children) {
		Destrutibles.Add(child->GetOwner());
	}
	Super::BeginPlay();

	// for (AActor* obj : spawnShapes) {
	// 	if (obj->GetName().StartsWith("S")) {
	// 		AddSquare(obj->GetActorLocation()-FVector(0,0,50), obj->GetActorScale().X * 100, obj->GetActorScale().X * 100);
	// 		obj->Destroy();
	// 		UE_LOG(LogTemp, Warning, TEXT("Number of areas %s"), *FString::FromInt(spawnAreas.Num()));
	// 		UE_LOG(LogTemp, Warning, TEXT("location of area %s"), *obj->GetActorScale().ToString());
	// 	}
	// }
}

// void ASpawnBreakables::AddSquare(FVector pos, int length, int width)
// {
// 	Square* newSquare = new Square(pos, length, width);
// 	spawnAreas.Add(newSquare);
// 	//UE_LOG(LogTemp, Warning, TEXT("Square %s"), *FString::FromInt(width));
// }
//
// void ASpawnBreakables::AddCircle(FVector pos, int radius)
// {
// 	Circle * newCircle = new Circle(pos, radius);
// 	spawnAreas.Add(newCircle);
// }
//
// void ASpawnBreakables::AddDonut(FVector pos, int outerRadius, int innerRadius)
// {
// 	Donut* newDonut = new Donut(pos, outerRadius, innerRadius);
// 	spawnAreas.Add(newDonut);
// }

ASpawnArea* ASpawnBreakables::getSpawnObject()
{
	if (spawnAreas.Num() != 0) {
		int area = FMath::RandRange(0, spawnAreas.Num() - 1);
		return spawnAreas[area];
	}
	else {
		return nullptr;
	}
	
}

FVector ASpawnBreakables::createSpawnLocation(int tries = 0)
{
	FVector loc = FVector(0,0,0);
	if (spawnAreas.Num() != 0) {
		if (spawnAreas.Num() != 1) {
			int area = FMath::RandRange(0, spawnAreas.Num() - 1);
			loc = spawnAreas[area]->randomPos();
		}
		else {
			loc = spawnAreas[0]->randomPos();
		}
	}
	else {
		return FVector(0, 0, 0);
	}
	FHitResult hit;
	bool hitResult = GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector(0,0,-100000), ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
	UPrimitiveComponent* test = hit.Component.Get();
	loc = hit.Location;
	if (test->IsSimulatingPhysics() == true) {
		if (tries < maxSpawnAttempts) {
			loc = createSpawnLocation(tries + 1);
		}
		else {
			return FVector(0, 0, 0);
		}
	}
	TArray<USceneComponent*> children;
	RootComponent->GetChildrenComponents(false, children);
	for (USceneComponent* child: children) {
		FVector childLoc = child->GetComponentLocation();
		if (FVector::Distance(childLoc, loc) < minDistance) {
			if (tries < maxSpawnAttempts) {
				loc = createSpawnLocation(tries + 1);
			}
			else {
				return FVector(0,0,0);
			}
		}
	}
	return loc;
}

// Called every frame
void ASpawnBreakables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
