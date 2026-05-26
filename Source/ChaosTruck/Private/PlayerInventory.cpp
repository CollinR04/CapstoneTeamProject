// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventory.h"
#include "PowerUp.h"
#include "PowerUpEffect.h"
#include "PowerUpScythe.h"
#include "PowerUpGhost.h"
#include "PowerUpFlameTrail.h"
#include "PowerUpCurse.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerInventory::UPlayerInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicatedByDefault(true);

	CanPickUp = true;
	CanUse = true;
	CurItemID = -1;
	CurItemName = "None";
}


// Called when the game starts
void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UPlayerInventory::PickUpItem(int ID, FName Name)
{
	// Set CurItemID to ID of item picked up
	if (CanPickUp)
	{
		CurItemID = ID;
		CurItemName = Name;
		CanPickUp = false;
		UE_LOG(LogTemp, Log, TEXT("(IconTest) Picked up power-up: %s"), *CurItemName.ToString());
	}
}

void UPlayerInventory::ServerUseItem_Implementation(bool Use, bool Other)
{
	UseItem(Use, Other);
}

void UPlayerInventory::UseItem(bool Use, bool Other)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		ServerUseItem(Use, Other);
		return;
	}

	// Stops process if ID is default (i.e. not holding anything)
	if (CurItemName == "None" || CurItemID == -1 || !Use)
	{
		UE_LOG(LogTemp, Warning, TEXT("Holding anything: %s"), (Use ? TEXT("true") : TEXT("false")));
		return;
	}

	AActor* OwnerActor = GetOwner();

	UClass* PowerUpClass = UPowerUpEffect::StaticClass();

	// Scythe
	if (CurItemName == "Scythe")
	{
		//UE_LOG(LogTemp, Warning, TEXT("Scythe"));

		PowerUpClass = UPowerUpScythe::StaticClass();
	}

	// Ghost
	else if (CurItemName == "Ghost")
	{
		//UE_LOG(LogTemp, Warning, TEXT("Ghost"));

		PowerUpClass = UPowerUpGhost::StaticClass();
	}

	// Fire Trail
	else if (CurItemName == "Trail")
	{
		//UE_LOG(LogTemp, Warning, TEXT("Fire Trail"));

		PowerUpClass = UPowerUpFlameTrail::StaticClass();
	}

	// Curse Bomb
	else if (CurItemName == "Curse")
	{
		//UE_LOG(LogTemp, Warning, TEXT("Curse Bomb"));

		PowerUpClass = UPowerUpCurse::StaticClass();
	}


	UPowerUpEffect* Effect = Cast<UPowerUpEffect>(
		OwnerActor->AddComponentByClass(PowerUpClass, false, FTransform::Identity, true)
	);

	if (Effect)
	{
		CanUse = false;
		Effect->RegisterComponent();
		Effect->Activate(true);
		Effect->SetComponentTickEnabled(true);
		// Assigns delegate to signal when trail is finished
		Effect->OnPowerUpFinished.AddUObject(this, &UPlayerInventory::ItemFinish);
		Effect->UsePowerUp(Other);
	}
}

void UPlayerInventory::FinishEarly()
{
	ItemFinish();
}

void UPlayerInventory::ItemFinish()
{
	// Return to defaults
	CanPickUp = true;
	CanUse = true;
	CurItemID = -1;
	CurItemName = "None";
	UE_LOG(LogTemp, Warning, TEXT("%s"), (CanUse ? TEXT("true") : TEXT("false")));

	// Check for power-ups
	TArray<AActor*> ItemCheck;
	GetOwner()->GetOverlappingActors(ItemCheck, APowerUp::StaticClass());
	for (AActor* Actor : ItemCheck)
	{
		APawn* PawnReference = Cast<APawn>(Actor);

		if (PawnReference)
		{
			APowerUp* PowerUp = Cast<APowerUp>(PawnReference);
			if (PowerUp)
			{
				static FName FunctionName("BP_CheckForItems");
				UFunction* EventFunc = PawnReference->GetInstigatorController()->FindFunction(FunctionName);
				if (EventFunc)
				{
					struct
					{
						AActor* Player;
						AActor* Item;
					} Params;

					Params.Player = GetOwner();
					Params.Item = PowerUp;
					GetOwner()->ProcessEvent(EventFunc, &Params);
				}
			}
		}
	}
}

void UPlayerInventory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerInventory, CanPickUp);
	DOREPLIFETIME(UPlayerInventory, CanUse);
	DOREPLIFETIME(UPlayerInventory, CurItemID);
}

