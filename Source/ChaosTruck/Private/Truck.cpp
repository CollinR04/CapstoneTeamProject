// Fill out your copyright notice in the Description page of Project Settings.


#include "Truck.h"

// Sets default values
ATruck::ATruck()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	// Create and set chasis as root node
	Chasis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chasis"));
	Chasis->SetSimulatePhysics(true);
	SetRootComponent(Chasis);

	// Calculate mass
	TArray<UPrimitiveComponent*> ComponentsWithMass;
	GetComponents<UPrimitiveComponent>(ComponentsWithMass);
	for (UPrimitiveComponent* Component : ComponentsWithMass)
	{
		Mass += Component->GetMass();
	}

	// Create and setup movement component
	MovementComponent = CreateDefaultSubobject<UTruckMovementComponent>(TEXT("TruckMovementComponent"));
	MovementComponent->SetMovementAnchor(Chasis);
	MovementComponent->SetMass(/*Chasis->GetMass()*/ 0.1);

}

// Called when the game starts or when spawned
void ATruck::BeginPlay()
{
	Super::BeginPlay();

	// Enable replication
	//SetReplicates(true);
	//SetReplicateMovement(true);
	
}

// Called every frame
void ATruck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTruckMovementComponent* ATruck::GetTruckMovementComponent()
{
	return MovementComponent;
}

