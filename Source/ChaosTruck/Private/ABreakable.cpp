// Fill out your copyright notice in the Description page of Project Settings.


#include "ABreakable.h"

// Sets default values
AABreakable::AABreakable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AABreakable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

