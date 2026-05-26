// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "PowerUpScythe.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHAOSTRUCK_API UPowerUpScythe : public UPowerUpEffect
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPowerUpScythe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ScytheActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> PlayerActor;

	UPROPERTY()
	AActor* ScytheInstance;

	// How fast scythe spins around itself
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpinSpeed;

	// How fast scythe spins around vehicle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitSpeed;

	// Distance of scythe from vehicle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RotationRate = FRotator(0.0f, 90.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator DeltaRotation = RotationRate * (1 / 60);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScytheDuration;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Starts process; called by PlayerInventory
	virtual void UsePowerUp(bool Back) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:

	float CurOrbitAngle;

	float CurLocalAngle;

	// Vertical distance of scythe from vehicle origin
	float HeightOffset;

	float TimeLeft;

};
