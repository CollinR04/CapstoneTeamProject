// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpEffect.h"

// Sets default values for this component's properties
UPowerUpEffect::UPowerUpEffect()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UPowerUpEffect::UsePowerUp(bool Back)
{

}


// Called when the game starts
void UPowerUpEffect::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UPowerUpEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

