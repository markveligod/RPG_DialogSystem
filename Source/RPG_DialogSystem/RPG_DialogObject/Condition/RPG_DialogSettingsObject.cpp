// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
#include "RPG_DialogObject/RPG_DialogObjectBase.h"

#pragma region ActionDialogEvent

void URPG_DialogEvent::TriggerEvent()
{
    Trigger_Event();
}

#pragma endregion

#pragma region ActionDialogTask

bool URPG_DialogCondition::IsCondition()
{
    return IsConditionNative();
}

bool URPG_DialogCondition::IsConditionNative_Implementation()
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

bool URPG_DialogSettingsObject::IsValidCondition() const
{
    for (auto Cond : ArrayCondition)
    {
        if (!Cond->IsCondition())
        {
            return false;
        }
    }
    return true;
}

void URPG_DialogSettingsObject::ExecuteEvents()
{
    for (const auto Event : ArrayEvent)
    {
        Event->TriggerEvent();
    }
}

#pragma endregion

