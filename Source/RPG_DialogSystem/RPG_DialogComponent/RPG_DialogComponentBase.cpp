// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogComponent/RPG_DialogComponentBase.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"

#if !UE_BUILD_SHIPPING

static TAutoConsoleVariable<bool> EnableD_DialogSystemShowDialogData(TEXT("DialogSystem.ShowDialogData"), false, TEXT("DialogSystem.ShowDialogData [true/false]"), ECVF_Cheat);

#endif

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

#if !UE_BUILD_SHIPPING

    const auto DialogSystemShowDialogData = IConsoleManager::Get().FindConsoleVariable(TEXT("DialogSystem.ShowDialogData"));
    if (GEngine && GetOwner() && !IsNetMode(NM_DedicatedServer) && DialogSystemShowDialogData && DialogSystemShowDialogData->GetBool())
    {
        FString Result;
        Result.Append(FString::Printf(TEXT("TargetDialog: [%s]\n"), *GetNameSafe(TargetDialog)));
        if (TargetDialog)
        {
            // if (URPG_DialogSettingsObject* TargetDialogSettings = TargetDialog->FindNodeByIndex(TargetDialog->TargetIDNode))
            // {
            //     Result.Append(FString::Printf(TEXT("Index node: [%i]\n"), TargetDialogSettings->IndexNode));
            //     Result.Append(FString::Printf(TEXT("NodePosition: [%s]\n"), *TargetDialogSettings->NodePosition.ToString()));
            //     Result.Append(FString::Printf(TEXT("TypeStateDialog: [%s]\n"), *UEnum::GetValueAsString(TargetDialogSettings->TypeStateDialog)));
            //     Result.Append(FString(TEXT("Out Node: ")));
            //     for (const int32 IDNode : TargetDialogSettings->OutNodes)
            //     {
            //         Result.Append(FString::FromInt(IDNode));
            //     }
            //     Result.Append(TEXT("\n"));
            // }
        }
    }

#endif
}

void URPG_DialogComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(URPG_DialogComponentBase, TargetDialog, COND_OwnerOnly, REPNOTIFY_Always);
}

// Called when the game starts
void URPG_DialogComponentBase::BeginPlay()
{
    Super::BeginPlay();
}

#pragma endregion

#pragma region Action

void URPG_DialogComponentBase::RunDialog(const TSoftObjectPtr<URPG_DialogObjectBase>& DialogObject)
{
    if (CLOG_DIALOG_SYSTEM(GetOwner() == nullptr, "Owner is nullptr")) return;
    if (CLOG_DIALOG_SYSTEM(TargetDialog != nullptr, "TargetDialog is exist")) return;
    const URPG_DialogObjectBase* TempDialogObject = DialogObject.IsValid() ? DialogObject.Get() : DialogObject.LoadSynchronous();
    if (CLOG_DIALOG_SYSTEM(TempDialogObject == nullptr, "Dialog class is nullptr")) return;

    if (GetOwner()->HasAuthority())
    {
        TargetDialog = DuplicateObject(TempDialogObject, this);
        TargetDialog->OnCompleteDialog.BindUObject(this, &ThisClass::RegisterCompleteObject);
        TargetDialog->InitDialog(Cast<APlayerController>(GetOwner()));
        NotifyUpdateTargetDialog();
    }
    else
    {
        ServerRunDialog(DialogObject.ToString());
    }
}

void URPG_DialogComponentBase::OnRep_TargetDialog()
{
    NotifyUpdateTargetDialog();
}

void URPG_DialogComponentBase::RegisterCompleteObject(URPG_DialogObjectBase* DialogObject)
{
    if (CLOG_DIALOG_SYSTEM(DialogObject == nullptr, "Dialog object is nullptr")) return;
    TargetDialog = nullptr;
    NotifyUpdateTargetDialog();
}

void URPG_DialogComponentBase::ServerRunDialog_Implementation(const FString& ObjectPath)
{
    RunDialog(TSoftObjectPtr<URPG_DialogObjectBase>(ObjectPath));
}

bool URPG_DialogComponentBase::ServerRunDialog_Validate(const FString& ObjectPath)
{
    return !ObjectPath.IsEmpty();
}

#pragma endregion

#pragma region Signature

void URPG_DialogComponentBase::NotifyUpdateTargetDialog()
{
    // OnUpdateTargetDialog.Broadcast(TargetDialog);
}

#pragma endregion
