// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABreakable::ABreakable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true; // UE5 component replication
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakable::Respawn_Implementation()
{
	if (HasAuthority()) {
		UE_LOG(LogTemp, Warning, TEXT("God Why"));
	}
	UGeometryCollectionComponent* oldGeo = Cast<UGeometryCollectionComponent>(UtheThing);
	if (oldGeo) {
		UGeometryCollectionComponent* newGeo = NewObject<UGeometryCollectionComponent>(this, UGeometryCollectionComponent::StaticClass(), TEXT("ANewGeometryCollection"));
		if (newGeo) {

			newGeo->SetupAttachment(this->GetRootComponent());
			newGeo->SetWorldLocation(this->GetTransform().GetLocation());
			newGeo->SetWorldRotation(this->GetTransform().GetRotation());
			newGeo->RestCollection = oldGeo->RestCollection;
			newGeo->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			newGeo->SetIsReplicated(true);
			newGeo->RegisterComponent();
			newGeo->PutRigidBodyToSleep();
			CurrentThing->DestroyComponent();
			CurrentThing = newGeo;

			AddReplicatedSubObject(CurrentThing);

		}
	}
	else {
		UStaticMeshComponent* oldMesh = Cast<UStaticMeshComponent>(UtheThing);
		if (oldMesh) {
			UStaticMeshComponent* newMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("ANewStaticMesh"));
			if (newMesh) {
				newMesh->SetupAttachment(this->GetRootComponent());
				newMesh->SetWorldLocation(this->GetTransform().GetLocation());
				newMesh->SetWorldRotation(this->GetTransform().GetRotation());
				newMesh->SetWorldScale3D(oldMesh->GetRelativeScale3D());
				newMesh->SetStaticMesh(oldMesh->GetStaticMesh());
				newMesh->SetMobility(EComponentMobility::Movable);
				newMesh->BodyInstance.SetMassOverride(1.f, true); // Set a mass
				newMesh->BodyInstance.LinearDamping = 0.1f;
				newMesh->BodyInstance.AngularDamping = 0.1f;
				newMesh->BodyInstance.bSimulatePhysics = true;
				newMesh->RegisterComponent();
				newMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				newMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
				newMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
				newMesh->SetSimulatePhysics(true);

				CurrentThing->DestroyComponent();
				CurrentThing = newMesh;
			}
		}
	}
}



// Called every frame
void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABreakable, CurrentThing);
}