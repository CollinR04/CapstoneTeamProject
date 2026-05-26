// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hazard.generated.h"

UCLASS()
class CHAOSTRUCK_API AHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHazard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hazard")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float SecondDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float ArmorDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	float DamageInterval;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Hazard")
	float GetArmorDamage() { return ArmorDamage; }
};
