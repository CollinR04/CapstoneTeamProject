// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerUpEffect.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPowerUpFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHAOSTRUCK_API UPowerUpEffect : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPowerUpEffect();

	virtual void UsePowerUp(bool Back);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PowerUp")
	FName PowerUpName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	int PowerUpID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float PowerUpDuration;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Returns ID of Power-up
	int getPowerUpID() { return PowerUpID; }

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	float getDuration() { return PowerUpDuration; }

	// Delegate that sends signal when power-up is finished being used
	FOnPowerUpFinished OnPowerUpFinished;
};
