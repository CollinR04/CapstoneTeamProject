// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorCollection.h"
#include "Truck.h"
#include "Hazard.h"
#include "TruckPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

// Sets default values for this component's properties
UArmorCollection::UArmorCollection()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


void UArmorCollection::TakeDamage(float Damage, AActor* OtherActor)
{
    // Check if collection is eligible for damage
    if (!ArmorList.IsEmpty() && !bHasIFrames)
    {
        UE_LOG(LogTemp, Warning, TEXT("Armor piece is taking damage"));
        // Deal damage to armor piece
        UArmor* NextArmor = *ArmorList.Peek();
        int DamageLeft = NextArmor->TakeDamage(Damage);
        //UE_LOG(LogTemp, Warning, TEXT("Current armor health %f"), (*ArmorList.Peek())->GetCurrentHealth());
        // Remove Armor from queue when destroyed
        if (NextArmor->GetCurrentHealth() <= 0.0f)
        {
            OnArmorBroken.Broadcast(NextArmor);
            //UE_LOG(LogTemp, Warning, TEXT("Armor health gone"));
            //NextArmor->DestroyComponent();
            ArmorList.Pop();
            if (ArmorList.IsEmpty())
            {
                OnCollectionEmpty.Broadcast(true);
            }
        }
        // Grant IFrames to this collection
        AddIFrames();
    }
    // Main truck should take damage instead
    else if (!bHasIFrames && !OtherActor->IsA(AHazard::StaticClass()))
    {
        // Call delegate for truck
        if (GetPhysicsLinearVelocity().Length() < OtherActor->GetVelocity().Length())
        {
            OnTruckDamage.Broadcast(HP_DAMAGE_FAST, OtherActor);
        }
        OnTruckDamage.Broadcast(HP_DAMAGE_SLOW, OtherActor);
        AddIFrames();
    }
}

void UArmorCollection::OnArmorCollision(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Don't deal damage if you hit yourself somehow
    //UE_LOG(LogTemp, Warning, TEXT("Calling collision with truck %s"), *GetOwner()->GetName());
    if (OtherActor != this->GetAttachParentActor() && (OtherActor->IsA(ATruck::StaticClass()) || OtherActor->IsA(AHazard::StaticClass())))
    {
        if (OtherActor->IsA(AHazard::StaticClass()))
        {
            OtherActor->GetOwner()->GetAttachedActors(HazardOwnerChildren);

            for (AActor* Child : HazardOwnerChildren)
            {
                if (Child == GetAttachParentActor())
                {
                    return;
                }
            }
        }
        if ((OtherActor->IsA(AHazard::StaticClass()) && OtherActor->GetOwner() != this->GetOwner())
            || !bIsFront || GetPhysicsLinearVelocity().Length() < SpeedThreshold)
        {
            if (OtherActor && OtherActor->GetOwner())
            {

                UE_LOG(LogTemp, Warning, TEXT("Calling collision between truck with owner %s and other owner of %s"), *GetOwner()->GetName(), *OtherActor->GetOwner()->GetName());
            }
            if (storedMode == 1) {
                TakeDamage(1.0f, OtherActor);
            }
            else if (storedMode == 2) {
                ValidateHit(1.0f, OtherActor);
            }
        }

    }

    /*else if (OtherActor->IsA(AHazard::StaticClass()))
    {
        AHazard* HazardActor = Cast<AHazard>(OtherActor);
        TakeDamage(HazardActor->GetArmorDamage(), OtherActor);
    }*/
}

// Called when the game starts
void UArmorCollection::BeginPlay()
{
	Super::BeginPlay();

	// Get all Armor child components
	TArray<USceneComponent*> ChildComponents;
	ChildComponents = GetAttachChildren();
    
	if (!ChildComponents.IsEmpty())
	{
        for (USceneComponent* ChildComponent : ChildComponents)
        {
            if (!ChildComponent->IsA(UArmor::StaticClass()))
            {
                ChildComponents.Remove(ChildComponent);
            }
        }
        // ChildComponents now only contains Armor components
        TArray<UArmor*> TempArmorComponenets;
        for (USceneComponent* ArmorComponent : ChildComponents)
        {
            TempArmorComponenets.Emplace(static_cast<UArmor*>(ArmorComponent));
        }
        // Sort armor by priority to know order of breakage
        TempArmorComponenets.Sort();
        // Create queue using items in order of breaking
        for (UArmor* ArmorComponent : TempArmorComponenets)
        {
            UE_LOG(LogTemp, Warning, TEXT("New armor named %s added with priority %d"), *ArmorComponent->GetName(), ArmorComponent->GetBreakPriority());
            ArmorList.Enqueue(ArmorComponent);
        }
	}

    // Register BoxComponent overlap delegate
    this->OnComponentBeginOverlap.AddDynamic(this, &UArmorCollection::OnArmorCollision);
}


void UArmorCollection::AddIFrames()
{
    bHasIFrames = true;
    GetWorld()->GetTimerManager().SetTimer(IFrameTimerHandle, this, &UArmorCollection::RemoveIFrames, IFrameDuration, false);
}

void UArmorCollection::RemoveIFrames()
{
    bHasIFrames = false;
}

// Called every frame
void UArmorCollection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UArmorCollection::ValidateHit(float Damage, AActor* OtherActor)
{
    int32 selfId = -1;
    int32 otherId = -1;


    //Get the owners TeamID
    AActor* shell = this->GetOwner();

    if (shell)
    {
        AActor* pawn = shell->GetAttachParentActor();
        if (pawn)
        {
            APawn* selfPawn = Cast<APawn>(pawn);
            if (selfPawn)
            {
                APlayerState* state = selfPawn->GetPlayerState();

                if (state) {
                    ATruckPlayerState* truckState = Cast<ATruckPlayerState>(state);

                    if (truckState) {
                        selfId = truckState->TeamID;
                    }
                    else {
                        UE_LOG(LogTemp, Warning, TEXT("Fail 5"));
                    }
                    //delete truckState;
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("Fail 4"));
                }
                //delete state;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Fail 3"));
            }
            //delete selfPawn;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Fail 2"));
        }
        //delete pawn;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Fail 1"));
    }
    //delete shell;


    //Get the others TeamID
    if (OtherActor) {
        APawn* otherPawn = Cast<APawn>(OtherActor);
        if (otherPawn) {
            APlayerState* otherState = otherPawn->GetPlayerState();

            if (otherState) {
                ATruckPlayerState* otherTruckState = Cast<ATruckPlayerState>(otherState);

                if (otherTruckState) {
                    otherId = otherTruckState->TeamID;
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("Fail 9"));
                }
                //delete otherTruckState;
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Fail 8"));
            }
            //delete otherState;
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Fail 7"));
        }
        //delete otherPawn;
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Fail 6"));
    }

    //Output
    UE_LOG(LogTemp, Warning, TEXT("Player Teams %i, %i"), selfId, otherId);

    if (selfId != -1 or otherId != -1) {
        if (selfId != otherId) {
            UE_LOG(LogTemp, Warning, TEXT("Player Hurt"));
            TakeDamage(Damage, OtherActor);
        }
    }
    else {
        TakeDamage(Damage, OtherActor);
    }
}