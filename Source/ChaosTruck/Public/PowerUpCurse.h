// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "PowerUpCurse.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSTRUCK_API UPowerUpCurse : public UPowerUpEffect
{
	GENERATED_BODY()

public:
	UPowerUpCurse();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	virtual void UsePowerUp(bool Back) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Curse")
	TSubclassOf<AActor> CurseBomb;

	UPROPERTY()
	AActor* BombInstance;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float ThrowDistance;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float DropDistance;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float ThrowHeight;

private:
	float TimeLeft;
};
