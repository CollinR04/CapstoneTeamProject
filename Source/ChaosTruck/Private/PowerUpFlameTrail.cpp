// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpFlameTrail.h"
#include "GameFramework/Actor.h"

UPowerUpFlameTrail::UPowerUpFlameTrail()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> FireBP(TEXT("/Game/Blueprints/PowerUps/BP_FireActor"));
	if (FireBP.Succeeded())
	{
		FireSegment = FireBP.Class;
	}

	PowerUpName = "Flame Trail";
	SpawnInterval = 0.1f;
	FireLife = 13.0f;
	PowerUpDuration = 15.0f;
	TimeLeft = 0.0f;

	SetIsReplicatedByDefault(true);
}

void UPowerUpFlameTrail::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner() || !GetOwner()->HasAuthority() || TimeLeft <= 0.0f)
	{
		return;
	}

	if (IntervalTimer >= SpawnInterval)
	{
		IntervalTimer = 0.0f;

		FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 500.0f + GetOwner()->GetActorRightVector() * 375.0f;
		FVector SpawnLocationOther = SpawnLocation - GetOwner()->GetActorRightVector() * 750.0f;
		FRotator SpawnRotation = GetOwner()->GetActorRotation();

		if (FireSegment)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			AActor* Fire = GetWorld()->SpawnActor<AActor>(FireSegment, SpawnLocation, SpawnRotation, SpawnParams);
			AActor* FireOther = GetWorld()->SpawnActor<AActor>(FireSegment, SpawnLocationOther, SpawnRotation, SpawnParams);

			if (Fire && FireOther)
			{
				// UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *Fire->GetActorLocation().ToString());
				// UE_LOG(LogTemp, Warning, TEXT("Other Location: %s"), *FireOther->GetActorLocation().ToString());
				Fire->SetLifeSpan(FireLife);
				FireOther->SetLifeSpan(FireLife);

				Fire->SetReplicates(true);
				Fire->SetReplicateMovement(true);
				FireOther->SetReplicates(true);
				FireOther->SetReplicateMovement(true);

				UStaticMeshComponent* FireCollision = Cast<UStaticMeshComponent>(Fire->FindComponentByClass<UStaticMeshComponent>());
				UStaticMeshComponent* FireCollisionOther = Cast<UStaticMeshComponent>(FireOther->FindComponentByClass<UStaticMeshComponent>());

				if (FireCollision && FireCollisionOther)
				{
					AActor* Self = GetOwner();
					FireCollision->IgnoreActorWhenMoving(Self, true);
					FireCollision->SetNotifyRigidBodyCollision(true);
					FireCollision->SetGenerateOverlapEvents(true);
					FireCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					FireCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
					FireCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

					FireCollisionOther->IgnoreActorWhenMoving(Self, true);
					FireCollisionOther->SetNotifyRigidBodyCollision(true);
					FireCollisionOther->SetGenerateOverlapEvents(true);
					FireCollisionOther->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					FireCollisionOther->SetCollisionResponseToAllChannels(ECR_Ignore);
					FireCollisionOther->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

				}
			}
		}
	}

	TimeLeft -= DeltaTime;
	IntervalTimer += DeltaTime;

	if (TimeLeft <= 0.0f)
	{
		OnPowerUpFinished.Broadcast();
	}
}

void UPowerUpFlameTrail::UsePowerUp_Implementation(bool Back)
{
	TimeLeft = PowerUpDuration;
	IntervalTimer = 0.0f;
}
