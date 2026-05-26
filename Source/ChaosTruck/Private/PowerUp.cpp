// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// UE_LOG(LogTemp, Warning, TEXT("PowerUp constructor: binding overlap"));
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	// RespawnPickup();
	// UE_LOG(LogTemp, Warning, TEXT("PowerUp BeginPlay called"));
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUp::OnRep_PowerUpNum()
{
	UpdatePickupMesh();
}

void APowerUp::OnRep_PowerUpName()
{
	UpdatePickupMesh();
}

void APowerUp::RespawnPickup()
{
	if (HasAuthority())
	{
		PowerUpNum = FMath::RandRange(0, 3);
		PowerUpName = NameList[PowerUpNum];
		WaitTime = FMath::RandRange(10.0f, 20.0f);
		OnRep_PowerUpNum();
	}
}

void APowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

}

void APowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APowerUp, PowerUpNum);
	DOREPLIFETIME(APowerUp, WaitTime);
}



