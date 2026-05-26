// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Containers/Queue.h"
#include "Armor.h"
#include "ArmorCollection.generated.h"


// Declare delegate to notify truck damage
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTruckDamageEvent, float, Damage, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollectionEmptyEvent, bool, bIsEmpty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArmorBreakEvent, UArmor*, BrokenArmor);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class CHAOSTRUCK_API UArmorCollection : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArmorCollection();

	UPROPERTY(EditAnywhere)
	float HP_DAMAGE_FAST{ 75.0f };
	UPROPERTY(EditAnywhere)
	float HP_DAMAGE_SLOW{ 25.0f };

	// Apply damage to proper armor pieces and handle overflow
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage, AActor* OtherActor);

	UFUNCTION()
	void OnArmorCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool bIsFront;

	// Speed at which the truck no longer takes damage in the front
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedThreshold;

	// Queue to store all armor pieces for this section of the shell
	TQueue<UArmor*> ArmorList;

	// Whether or not this collection can take damage
	bool bHasIFrames{ false };

	// Time IFrames lasts in seconds
	UPROPERTY(EditAnywhere)
	float IFrameDuration{ 0.7f };

	// IFrame timer handle
	FTimerHandle IFrameTimerHandle;

	// IFrames enabled
	void AddIFrames();

	// IFrame disabled
	void RemoveIFrames();

	UPROPERTY(BlueprintAssignable)
	FTruckDamageEvent OnTruckDamage;
	
	UPROPERTY(BlueprintAssignable)
	FCollectionEmptyEvent OnCollectionEmpty;

	UPROPERTY(BlueprintAssignable)
	FArmorBreakEvent OnArmorBroken;

	void ValidateHit(float Damage, AActor* OtherActor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	int32 storedMode;

	TArray<AActor*> HazardOwnerChildren;
};
