// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TruckMovementComponent.h"
#include "Truck.generated.h"

UCLASS()
class CHAOSTRUCK_API ATruck : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATruck();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	UTruckMovementComponent* GetTruckMovementComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UTruckMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Chasis;

private:
	// Total mass of all attached components
	float Mass{ 0.0f };
};
