// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"

#pragma region ActionDialogEvent

void URPG_DialogEvent::TriggerCondition()
{
    TriggerCondition_Event();
}

#pragma endregion

#pragma region ActionDialogTask

bool URPG_DialogCondition::IsConditionTask()
{
    return IsConditionTask_Event();
}

bool URPG_DialogCondition::IsConditionTask_Event_Implementation()
{
    return true;
}

#pragma endregion

#pragma region ActionDialog


#pragma endregion
