// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogComponent/RPG_DialogComponentBase.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/HUD.h"
#include "Net/UnrealNetwork.h"
#include "Net/Subsystems/NetworkSubsystem.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_Library/RPG_DialogFunctionLibrary.h"

#if !UE_BUILD_SHIPPING

static TAutoConsoleVariable<bool> EnableD_DialogSystemShowDialogData(TEXT("DialogSystem.ShowDialogData"), false, TEXT("DialogSystem.ShowDialogData [true/false]"), ECVF_Cheat);

#endif

#pragma region Default

// Sets default values for this component's properties
URPG_DialogComponentBase::URPG_DialogComponentBase()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

    SetIsReplicatedByDefault(false);
    bReplicateUsingRegisteredSubObjectList = false;
}

// Called when the game starts
void URPG_DialogComponentBase::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner() && GetOwner()->IsNetMode(NM_DedicatedServer))
    {
        TargetDialog = nullptr;
    }
}

#pragma endregion

#pragma region Action

void URPG_DialogComponentBase::RunDialog()
{
    if (CLOG_DIALOG_SYSTEM(TargetDialog == nullptr, "TargetDialog is nullptr")) return;
    if (CLOG_DIALOG_SYSTEM(GetOwner() == nullptr, "Owner is nullptr")) return;

    RunDialog_Internal();
}

void URPG_DialogComponentBase::RunDialog_Internal()
{
    if (CLOG_DIALOG_SYSTEM(GetWorld() == nullptr, "World is nullptr")) return;

    TargetDialog->ResetDialog();
    if (TargetDialog->InitDialog(GetOwner()))
    {
        NotifyUpdateTargetDialog();
        TargetDialog->OnUpdateDialog.BindUObject(this, &ThisClass::RegisterUpdateDialogObject);
        GetWorld()->GetTimerManager().SetTimerForNextTick(TargetDialog.Get(), &URPG_DialogObjectBase::RunDialog);
    }
}

void URPG_DialogComponentBase::RegisterUpdateDialogObject(URPG_DialogObjectBase* DialogObject)
{
    if (CLOG_DIALOG_SYSTEM(DialogObject == nullptr, "Dialog object is nullptr")) return;
    if (CLOG_DIALOG_SYSTEM(GetWorld() == nullptr, "World is nullptr")) return;

    NotifyUpdateTargetDialog();
}

#pragma endregion

#pragma region Signature

void URPG_DialogComponentBase::NotifyUpdateTargetDialog()
{
    OnUpdateTargetDialog.Broadcast(this);
}

#pragma endregion
