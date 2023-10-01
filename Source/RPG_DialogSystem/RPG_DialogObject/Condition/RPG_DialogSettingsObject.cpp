// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"

#pragma region ActionDialogEvent

void URPG_DialogEvent::TriggerEvent(APlayerController* PC)
{
    Trigger_Event(PC);
}

#pragma endregion

#pragma region ActionDialogTask

bool URPG_DialogCondition::IsCondition(APlayerController* PC)
{
    return IsConditionNative(PC);
}

bool URPG_DialogCondition::IsConditionNative_Implementation(APlayerController* PC)
{
    return true;
}

#pragma endregion

#pragma region ActionDialog

TArray<FName> URPG_DialogSettingsObject::GetAllDialogPlayerAndNPCNames() const
{
    TArray<FName> Array;

    if (URPG_DialogObjectBase* DialogObjectBase = Cast<URPG_DialogObjectBase>(GetOuter()))
    {
        const TArray<URPG_DialogSettingsObject*>& ArrayDialogSettings = DialogObjectBase->GetArrayDialogNode();
        for (const auto DialogSetting : ArrayDialogSettings)
        {
            if (!DialogSetting) continue;
            if (DialogSetting->TypeStateDialog == ERPG_TypeStateDialog::PlayerNode || DialogSetting->TypeStateDialog == ERPG_TypeStateDialog::NPCNode)
            {
                FString Result = FString::Printf(TEXT("#%i | %s..."), DialogSetting->IndexNode, *DialogSetting->GetTextDialog().ToString().Mid(0,15));
                Array.Add(FName(Result));
            }
        }
    }

    return Array;
}

int32 URPG_DialogSettingsObject::GetIndexTransferNode() const
{
    if (TypeStateDialog == ERPG_TypeStateDialog::Transfer)
    {
        return FCString::Atoi(*NameTransfer.ToString());
    }
    return INDEX_NONE;
}

bool URPG_DialogSettingsObject::IsValidCondition(APlayerController* PlayerController) const
{
    for (auto Cond : ArrayCondition)
    {
        if (!Cond->IsCondition(PlayerController))
        {
            return false;
        }
    }
    return true;
}

void URPG_DialogSettingsObject::ExecuteEvents(APlayerController* PlayerController)
{
    for (const auto Event : ArrayEvent)
    {
        Event->TriggerEvent(PlayerController);
    }
}

#pragma endregion

