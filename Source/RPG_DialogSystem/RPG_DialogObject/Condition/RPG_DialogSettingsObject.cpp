// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"

#pragma region ActionDialogEvent

void URPG_DialogConditionEvent::TriggerCondition()
{
    TriggerCondition_Event();
}

#pragma endregion

#pragma region ActionDialogTask

bool URPG_DialogConditionTask::IsConditionTask()
{
    return IsConditionTask_Event();
}

bool URPG_DialogConditionTask::IsConditionTask_Event_Implementation()
{
    return true;
}

#pragma endregion
