// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogWidget.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeWork.h"

void URPG_DialogWidget::InitWidget(URPG_DialogObjectBase* DialogObject)
{
    if (CLOG_DIALOG_SYSTEM(DialogObject == nullptr, "DialogObject is nullptr")) return;
    TargetDialog = DialogObject;
    TargetDialog->OnUpdateDialog.BindUObject(this, &ThisClass::RegisterUpdateStateDialog);

    if (CLOG_DIALOG_SYSTEM(GetWorld() == nullptr, "World is nullptr")) return;
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitDialogWidget);
}

void URPG_DialogWidget::NextDialogNPC()
{
    ++ViewDialogNPCIndex;
    if (DialogNPC.IsValidIndex(ViewDialogNPCIndex) && DialogNPC[ViewDialogNPCIndex].Get())
    {
        DialogNPC[ViewDialogNPCIndex]->ExecuteEvents(GetOwningPlayer());
        ViewTextNPC(DialogNPC[ViewDialogNPCIndex]->GetText());
    }
    else
    {
        TMap<int32, FText> TempMap;
        for (int32 i = 0; i < DialogPlayer.Num(); ++i)
        {
            if (!DialogPlayer.IsValidIndex(i)) continue;
            URPG_DialogPlayer* Dialog = DialogPlayer[i].Get();
            if (!Dialog) continue;
            TempMap.Add(i, Dialog->GetText());
        }
        ViewTextPlayer(TempMap);
    }
}

bool URPG_DialogWidget::MakeSelectDialogPlayer(int32 Index)
{
    if (Index == INDEX_NONE) return false;
    if (!DialogPlayer.IsValidIndex(Index)) return false;
    URPG_DialogPlayer* Dialog = DialogPlayer[Index].Get();
    if (!Dialog) return false;
    URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(TargetDialogNode.Get());
    if (!NodeWork) return false;
    NodeWork->MakeSelectPlayerNode(Dialog);
    TargetDialogNode = nullptr;
    TargetDialog->NextDialog();
    return true;
}

void URPG_DialogWidget::RegisterUpdateStateDialog(URPG_DialogObjectBase* DialogObject)
{
    if (!GetWorld()) return;
    if (TargetDialog != DialogObject) return;
    if (DialogObject->GetDialogState() == ERPG_DialogObjectState::Finish)
    {
        FinishDialogObject();
        return;
    }

    URPG_DialogNodeBase* DialogNodeBase = TargetDialog->FindTargetNode();
    if (!DialogNodeBase) return;
    if (TargetDialogNode == DialogNodeBase) return;

    TargetDialogNode = DialogNodeBase;
    if (URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(TargetDialogNode.Get()))
    {
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::ChangeDialogNode);

        DialogNPC.Empty();
        DialogPlayer.Empty();
        TArray<URPG_DialogNPC*> ArrayNPCDialogs = NodeWork->GetNPCDialogs(GetOwningPlayer());
        TArray<URPG_DialogPlayer*> ArrayPlayerDialogs = NodeWork->GetPlayerDialogs(GetOwningPlayer());

        for (auto* Dialog : ArrayNPCDialogs)
        {
            DialogNPC.Add(Dialog);
        }
        for (auto* Dialog : ArrayPlayerDialogs)
        {
            DialogPlayer.Add(Dialog);
        }
        ViewDialogNPCIndex = INDEX_NONE;
    }
}