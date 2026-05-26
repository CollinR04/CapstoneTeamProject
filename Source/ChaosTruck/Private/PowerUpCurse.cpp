// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpCurse.h"
#include "Net/UnrealNetwork.h"

UPowerUpCurse::UPowerUpCurse()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> CurseBP(TEXT("/Game/VFX/ParticleSystems/BP_Chain"));
	if (CurseBP.Succeeded())
	{
		CurseBomb = CurseBP.Class;
		//UE_LOG(LogTemp, Warning, TEXT("CurseBP found at: %s"), *CurseBP.Class->GetPathName());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("CurseBP FAILED"));
	}

	ThrowDistance = -1600000.0f;
	DropDistance = 300000.0f;
	ThrowHeight = 50000.0f;
	PowerUpDuration = 15.0f;
	TimeLeft = 0.0f;

	SetIsReplicatedByDefault(true);

	//UE_LOG(LogTemp, Warning, TEXT("CurseBomb path = %s"), CurseBP.Class ? *CurseBP.Class->GetName() : TEXT("NULL"));
	//UE_LOG(LogTemp, Warning, TEXT("CurseBomb class = %s"), *GetPathName(CurseBomb));
	/*UE_LOG(LogTemp, Warning, TEXT("CurseBomb class is: %s"),
		CurseBP.Class ? *CurseBP.Class->GetPathName() : TEXT("NULL"));*/
}

void UPowerUpCurse::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPowerUpCurse::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPowerUpCurse, ThrowDistance);
	DOREPLIFETIME(UPowerUpCurse, DropDistance);
	DOREPLIFETIME(UPowerUpCurse, ThrowHeight);
}

void UPowerUpCurse::UsePowerUp_Implementation(bool Back)
{
	if (!GetOwner()->HasAuthority()) return;

	TimeLeft = PowerUpDuration;

	if (!CurseBomb) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorUpVector() * 1700.0f);
	FVector OtherSpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 1200.0f) + (GetOwner()->GetActorUpVector() * 500.0f);
	FRotator SpawnRotation = GetOwner()->GetActorRotation();

	if (!Back)
	{
		BombInstance = GetWorld()->SpawnActor<AActor>(CurseBomb, SpawnLocation, SpawnRotation, SpawnParams);
	}
	else
	{
		BombInstance = GetWorld()->SpawnActor<AActor>(CurseBomb, OtherSpawnLocation, SpawnRotation, SpawnParams);
	}

	if (!BombInstance) return;

	BombInstance->SetReplicates(true);
	BombInstance->SetReplicateMovement(true);

	UStaticMeshComponent* BombCollision = Cast<UStaticMeshComponent>(BombInstance->FindComponentByClass<UStaticMeshComponent>());

	if (!BombCollision) return;

	BombCollision->SetSimulatePhysics(true);
	FVector ThrowDirection;
	if (!Back)
	{
		UE_LOG(LogTemp, Warning, TEXT("Throw forward"));
		ThrowDirection = FVector(GetOwner()->GetActorForwardVector() * -ThrowDistance + FVector::UpVector * ThrowHeight);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Throw back"));
		ThrowDirection = FVector(GetOwner()->GetActorForwardVector() * DropDistance);
	}
	BombCollision->AddImpulse(ThrowDirection);

	OnPowerUpFinished.Broadcast();
}

