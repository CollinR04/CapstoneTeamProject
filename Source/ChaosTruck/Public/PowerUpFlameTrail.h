// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "PowerUpFlameTrail.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHAOSTRUCK_API UPowerUpFlameTrail : public UPowerUpEffect
{
	GENERATED_BODY()

public:
	UPowerUpFlameTrail();

	UFUNCTION(Server, Reliable)
	virtual void UsePowerUp(bool Back) override;
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category="Flame Trail")
	TSubclassOf<AActor> FireSegment;

	UPROPERTY(EditDefaultsOnly, Category = "Flame Trail")
	float SpawnInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Flame Trail")
	float FireLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TrailDuration;

private:
	float IntervalTimer;
	float TimeLeft;
};
