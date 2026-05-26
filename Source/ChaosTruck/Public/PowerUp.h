// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerInventory.h"
#include "PowerUpEffect.h"
#include "PowerUp.generated.h"

UCLASS()
class CHAOSTRUCK_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	TSubclassOf<UActorComponent> PowerUpEffect;

	UPROPERTY(ReplicatedUsing = OnRep_PowerUpNum, EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	int PowerUpNum;

	UPROPERTY(ReplicatedUsing = OnRep_PowerUpName, EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	FName PowerUpName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float WaitTime;

	FName NameList[4] = { "Scythe", "Ghost", "Trail", "Curse" };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FPowerUpData PowerUpData;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	/*UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;*/

	UFUNCTION()
	void OnRep_PowerUpNum();

	UFUNCTION()
	void OnRep_PowerUpName();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePickupMesh();

	UFUNCTION(BlueprintCallable)
	void RespawnPickup();

	// Detects when power-up is overlapped
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
