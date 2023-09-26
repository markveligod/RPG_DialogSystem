// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogComponent/RPG_DialogComponentBase.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "RPG_DialogObject/RPG_DialogObjectBase.h"

#pragma region Default

// Sets default values for this component's properties
URPG_DialogComponentBase::URPG_DialogComponentBase()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bAllowTickOnDedicatedServer = true;

#if !UE_BUILD_SHIPPING

    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

#endif
}

// Called every frame
void URPG_DialogComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void URPG_DialogComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(URPG_DialogComponentBase, TargetDialog, COND_OwnerOnly, REPNOTIFY_Always);
}

bool URPG_DialogComponentBase::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
    bool bState = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
    if (TargetDialog && TargetDialog->IsNetworkUpdate())
    {
        TargetDialog->ResetDialog();
        bState |= Channel->ReplicateSubobject(TargetDialog, *Bunch, *RepFlags);
        
        // ReSharper disable once CppDeprecatedEntity
        bState |= Channel->ReplicateSubobjectList(TargetDialog->ArrayDialogNode, *Bunch, *RepFlags);
    }
    return bState;
}

// Called when the game starts
void URPG_DialogComponentBase::BeginPlay()
{
    Super::BeginPlay();


}

#pragma endregion

#pragma region Action

void URPG_DialogComponentBase::OnRep_TargetDialog()
{
    NotifyUpdateTargetDialog();
}

#pragma endregion

#pragma region Signature

void URPG_DialogComponentBase::NotifyUpdateTargetDialog()
{
    OnUpdateTargetDialog.Broadcast(TargetDialog);
}

#pragma endregion
