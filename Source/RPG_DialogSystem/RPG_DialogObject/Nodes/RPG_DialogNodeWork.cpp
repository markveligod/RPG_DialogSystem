// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogNodeWork.h"

#pragma region DialogCondition

bool URPG_DialogCondition::IsCondition(APlayerController* PC)
{
    return IsConditionNative(PC);
}

bool URPG_DialogCondition::IsConditionNative_Implementation(APlayerController* PC)
{
    return true;
}

#pragma endregion

#pragma region DialogEvent

void URPG_DialogEvent::TriggerEvent(APlayerController* PC)
{
    Trigger_Event(PC);
}

#pragma endregion

#pragma region DialogText

void URPG_DialogText::Reset()
{
    Text = FText();
    auto FuncClear = [](TArray<UObject*> Array)
    {
        for (auto* El : Array)
        {
            El->MarkAsGarbage();
            El = nullptr;
        }
    };
    FuncClear(TArray<UObject*>(ArrayCondition));
    ArrayCondition.Empty();
    FuncClear(TArray<UObject*>(ArrayEvent));
    ArrayEvent.Empty();
}

FText& URPG_DialogText::GetText()
{
    return Text;
}

bool URPG_DialogText::IsHaveSomeCondition() const
{
    return ArrayCondition.Num() != 0;
}

bool URPG_DialogText::IsHaveSomeEvent() const
{
    return ArrayEvent.Num() != 0;
}

bool URPG_DialogText::IsValidCondition(APlayerController* PlayerController) const
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

void URPG_DialogText::ExecuteEvents(APlayerController* PlayerController)
{
    for (const auto Event : ArrayEvent)
    {
        Event->TriggerEvent(PlayerController);
    }
}

#pragma endregion

#pragma region DialogNPC

#pragma endregion

#pragma region DialogPlayer

void URPG_DialogPlayer::Reset()
{
    Super::Reset();
    NextIDNode = INDEX_NONE;
}

int32 URPG_DialogPlayer::GetNextIDNode() const
{
    return NextIDNode;
}

void URPG_DialogPlayer::SetNextIDNode(const int32 ID)
{
    NextIDNode = ID;
}

#pragma endregion

#pragma region DialogAction

URPG_DialogNodeWork::URPG_DialogNodeWork()
{
    SetTypeDialogNode(ERPG_TypeDialogNode::Work);
}

int32 URPG_DialogNodeWork::GetNextIDNode()
{
    if (CLOG_DIALOG_SYSTEM(ArrayDialogPlayer.Num() == 0, "ArrayDialogPlayer is empty")) return INDEX_NONE;
    if (CLOG_DIALOG_SYSTEM(!ArrayDialogPlayer.IsValidIndex(IndexSelectPlayerNode), "Selected index %i is not valid", IndexSelectPlayerNode)) return INDEX_NONE;

    URPG_DialogPlayer* DialogPlayer = ArrayDialogPlayer[IndexSelectPlayerNode];
    if (CLOG_DIALOG_SYSTEM(DialogPlayer == nullptr, "DialogPlayer by index: %i is not valid", IndexSelectPlayerNode)) return INDEX_NONE;

    return DialogPlayer->GetNextIDNode();
}

void URPG_DialogNodeWork::InitNode()
{
    Super::InitNode();

    IndexSelectPlayerNode = 0;
}

void URPG_DialogNodeWork::ResetNode()
{
    Super::ResetNode();

    auto FuncClear = [](TArray<URPG_DialogText*> Array)
    {
        for (auto* El : Array)
        {
            El->Reset();
            El = nullptr;
        }
    };

    FuncClear(TArray<URPG_DialogText*>(ArrayDialogPlayer));
    ArrayDialogPlayer.Empty();
    FuncClear(TArray<URPG_DialogText*>(ArrayDialogNPC));
    ArrayDialogNPC.Empty();
}

void URPG_DialogNodeWork::MakeSelectPlayerNode(uint16 SelectIndex)
{
    IndexSelectPlayerNode = SelectIndex;
}

void URPG_DialogNodeWork::MakeSelectPlayerNode(URPG_DialogPlayer* DialogPlayer)
{
    MakeSelectPlayerNode(ArrayDialogPlayer.IndexOfByKey(DialogPlayer));
}

TArray<URPG_DialogNPC*> URPG_DialogNodeWork::GetNPCDialogs(APlayerController* PlayerController)
{
    TArray<URPG_DialogNPC*> Dialogs;
    for (auto* Dialog : ArrayDialogNPC)
    {
        if (!Dialog) continue;
        if (Dialog->IsHaveSomeCondition() && !Dialog->IsValidCondition(PlayerController)) continue;
        Dialogs.Add(Dialog);
    }
    return Dialogs;
}

TArray<URPG_DialogPlayer*> URPG_DialogNodeWork::GetPlayerDialogs(APlayerController* PlayerController)
{
    TArray<URPG_DialogPlayer*> Dialogs;
    for (auto* Dialog : ArrayDialogPlayer)
    {
        if (!Dialog) continue;
        if (Dialog->IsHaveSomeCondition() && !Dialog->IsValidCondition(PlayerController)) continue;
        Dialogs.Add(Dialog);
    }
    return Dialogs;
}

#if WITH_EDITOR

const TArray<URPG_DialogPlayer*>& URPG_DialogNodeWork::GetOutputPinDialogs()
{
    return ArrayDialogPlayer;
}

const TArray<URPG_DialogNPC*>& URPG_DialogNodeWork::GetOutputNPCDialogs()
{
    return ArrayDialogNPC;
}

int32 URPG_DialogNodeWork::GetNextIDNodeByDialogPlayer(const uint16 IndexArray)
{
    if (CLOG_DIALOG_SYSTEM(ArrayDialogPlayer.Num() == 0, "ArrayDialogPlayer is empty")) return INDEX_NONE;
    if (CLOG_DIALOG_SYSTEM(ArrayDialogPlayer.IsValidIndex(IndexArray), "Selected index %i is not valid", IndexArray)) return INDEX_NONE;
    URPG_DialogPlayer* DialogPlayer = ArrayDialogPlayer[IndexArray];
    if (CLOG_DIALOG_SYSTEM(DialogPlayer == nullptr, "DialogPlayer by index: %i is not valid", IndexArray)) return INDEX_NONE;

    return DialogPlayer->GetNextIDNode();
}

void URPG_DialogNodeWork::SetNextIDNodeByDialogPlayer(const uint16 IndexArray, const int32 ID)
{
    if (CLOG_DIALOG_SYSTEM(ArrayDialogPlayer.Num() == 0, "ArrayDialogPlayer is empty")) return;
    if (CLOG_DIALOG_SYSTEM(ArrayDialogPlayer.IsValidIndex(IndexArray), "Selected index %i is not valid", IndexArray)) return;
    URPG_DialogPlayer* DialogPlayer = ArrayDialogPlayer[IndexArray];
    if (CLOG_DIALOG_SYSTEM(DialogPlayer == nullptr, "DialogPlayer by index: %i is not valid", IndexArray)) return;
    DialogPlayer->SetNextIDNode(ID);
}

#endif

#pragma endregion
