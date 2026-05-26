// Fill out your copyright notice in the Description page of Project Settings.


#include "Armor.h"
#include <Net/UnrealNetwork.h>

UArmor::UArmor()
{
    // Ensure that this component replicates
    this->SetIsReplicatedByDefault(true);
    //this->SetSimulatePhysics(false);
}

float UArmor::TakeDamage(float Damage)
{
    UE_LOG(LogTemp, Warning, TEXT("Truck %s TakeDamage called with damage: %f"), *GetOwner()->GetName(), Damage);
    // Need to carry over damage to next piece/car health
    AActor* Owner{ GetOwner() };
    if (Damage >= CurrentHealth)
    {
        if (Owner && Owner->HasAuthority())
        {
            Server_SetCurrentHealth(this, 0.0f);
            Server_BreakOff(this);
        }
        return Damage - CurrentHealth;
    }
    // Piece won't break so just do damage as normal
    else
    {
        if (Owner && Owner->HasAuthority())
        {
            Server_SetCurrentHealth(this, CurrentHealth - Damage);
        }
        // Show that piece has reached half health
        //if (CurrentHealth <= MaxHealth / 2.0f)
        //{
        //    SwapToBroken();
        //
        return 0.0f;
    }
}

void UArmor::Server_SetCurrentHealth_Implementation(UArmor* Armor, float NewHealth)
{
    if (Armor)
    {
        Armor->SetCurrentHealth(NewHealth);
    }
}

bool UArmor::Server_SetCurrentHealth_Validate(UArmor* Armor, float NewHealth)
{
    if (Armor && NewHealth >= 0.0f && NewHealth <= Armor->MaxHealth)
    {
        return true;
    }
    return false;
}

void UArmor::Multicast_BreakOff_Implementation(UArmor* Armor)
{
    UE_LOG(LogTemp, Warning, TEXT("Multicasting break"));
    Armor->SetStaticMesh(nullptr);
}

void UArmor::SwapToBroken()
{
    if (DamagedMesh != nullptr)
    {

        this->SetStaticMesh(DamagedMesh);
    }

}

void UArmor::BreakOff()
{
    this->SetStaticMesh(nullptr);
}

bool UArmor::Server_SetMaxHealth_Validate(UArmor* Armor, float NewMaxHealth)
{
    if (Armor && NewMaxHealth > 0.0f)
    {
        return true;
    }
    return false;
}

void UArmor::Server_SetMaxHealth_Implementation(UArmor* Armor, float NewMaxHealth)
{
    if (Armor)
    {
        Armor->MaxHealth = NewMaxHealth;
    }
}

float UArmor::GetMaxHealth() const
{
    return MaxHealth;
}

int UArmor::GetBreakPriority() const
{
    return BreakPriority;
}

float UArmor::GetCurrentHealth() const
{
    return CurrentHealth;
}

void UArmor::BeginPlay()
{
    Super::BeginPlay();


    // Initialize vars
    AActor* Owner{ GetOwner() };
    if (Owner && Owner->HasAuthority())
    {
        // Temp solution for SGX, fix later
        if (BreakPriority == 10)
        {
            MaxHealth += 1;
        }
        Server_SetCurrentHealth(this, MaxHealth);
        //CurrentHealth = MaxHealth;
    }
    //else
    //{
    //    CurrentHealth = MaxHealth;
    //}

}

bool UArmor::Server_BreakOff_Validate(UArmor* Armor)
{
    if (Armor)
    {
        return true;
    }
    return false;
}

void UArmor::Server_BreakOff_Implementation(UArmor* Armor)
{
    if (Armor)
    {
        //Armor->SetStaticMesh(nullptr);
        Armor->SetVisibility(false);
    }
    //Multicast_BreakOff(Armor);
}


void UArmor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UArmor, MaxHealth);
    DOREPLIFETIME(UArmor, CurrentHealth);
}

bool operator<(const UArmor& l, const UArmor& r)
{
    return std::tie(l.BreakPriority, l.CurrentHealth, l.MaxHealth) < std::tie(r.BreakPriority, r.CurrentHealth, r.MaxHealth);
}
