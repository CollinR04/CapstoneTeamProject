// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Breakable.generated.h"

UCLASS()
class CHAOSTRUCK_API ABreakable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Respawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* UtheThing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	USceneComponent* CurrentThing;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
