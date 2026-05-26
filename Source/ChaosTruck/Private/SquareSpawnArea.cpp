// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareSpawnArea.h"
#include "SpawnBreakables.h"

FVector ASquareSpawnArea::randomPos(int tries)
{
	int length = GetTransform().GetScale3D().X * 100;
	int width = GetTransform().GetScale3D().Y * 100;
	float randLength = FMath::RandRange(0, length) - length/2;
	float randWidth = FMath::RandRange(0, width) - width/2;

	FVector offset = FVector(randLength, randWidth, 0);
	offset = offset.RotateAngleAxis(GetActorRotation().Yaw, FVector(0, 0, 1));
	
	FVector loc = offset + GetActorLocation() - FVector(0,0,100);
	FHitResult hit;
	bool hitResult = GetWorld()->LineTraceSingleByChannel(hit, loc, loc + FVector(0, 0, -100000), ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
	UPrimitiveComponent* test = hit.Component.Get();
	loc = hit.Location;
	if (test->IsSimulatingPhysics() == true) {
		if (tries < maxSpawnAttempts) {
			loc = randomPos(tries + 1);
		}
		else {
			return FVector(0, 0, 0);
		}
	}
	ASpawnBreakables* castSpawner = Cast<ASpawnBreakables>(spawner);
	TArray<AActor*> children = castSpawner->getChildren();
	UE_LOG(LogTemp, Warning, TEXT("Num %i"), children.Num());
	for (AActor* child : children) {
		if (child != NULL) {
			FVector childLoc = child->GetActorLocation();
			if (FVector::Distance(childLoc, loc) < minDistance) {
				if (tries < maxSpawnAttempts) {
					loc = randomPos(tries + 1);
				}
				else {
					return FVector(0, 0, 0);
				}
			}
		}
	}
	return loc;
}