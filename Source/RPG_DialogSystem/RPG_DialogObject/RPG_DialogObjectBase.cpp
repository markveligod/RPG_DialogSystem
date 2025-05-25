// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "GameFramework/HUD.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "Nodes/RPG_DialogNodeFinish.h"
#include "Nodes/RPG_DialogNodeStart.h"
#include "Nodes/RPG_DialogNodeTransfer.h"
#include "Nodes/RPG_DialogNodeWork.h"
#include "RPG_DialogSystem/RPG_Interface/DialogActionInterface.h"

#pragma region Default

URPG_DialogObjectBase::URPG_DialogObjectBase()
{
    if (HasAnyFlags(RF_ClassDefaultObject)) return;

#if WITH_EDITOR

    if (!FindStartNode())
    {
        URPG_DialogObjectBase::CreateNode(ERPG_TypeDialogNode::Start, FVector2D(0.0f, 0.0f));
    }

#endif
}

void URPG_DialogObjectBase::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
}

UWorld* URPG_DialogObjectBase::GetWorld() const
{
    if (Owner.Get())
    {
        return Owner->GetWorld();
    }
    return UObject::GetWorld();
}

#pragma endregion

#pragma region Action

#pragma region FindNode

URPG_DialogNodeBase* URPG_DialogObjectBase::FindNodeByIndex(int32 IndexNode)
{
    for (auto* DialogNode : ArrayDialogNode)
    {
        if (!DialogNode) continue;
        if (DialogNode->GetIndexNode() == IndexNode) return DialogNode;
    }
    return nullptr;
}

URPG_DialogNodeBase* URPG_DialogObjectBase::FindTargetNode()
{
    if (CLOG_DIALOG_SYSTEM(TargetIDNode == INDEX_NONE, "Target id node is not valid")) return nullptr;
    return FindNodeByIndex(TargetIDNode);
}

URPG_DialogNodeBase* URPG_DialogObjectBase::FindStartNode()
{
    for (auto* DialogNode : ArrayDialogNode)
    {
        if (!DialogNode) continue;
        if (DialogNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Start) return DialogNode;
    }

    return nullptr;
}

TArray<URPG_DialogNodeBase*> URPG_DialogObjectBase::FindNodeByType(ERPG_TypeDialogNode TypeDialogNode)
{
    TArray<URPG_DialogNodeBase*> ArrayResult;
    for (auto* DialogNode : ArrayDialogNode)
    {
        if (!DialogNode) continue;
        if (DialogNode->GetTypeDialogNode() == TypeDialogNode)
        {
            ArrayResult.Add(DialogNode);
        }
    }
    return ArrayResult;
}

#pragma endregion

#pragma region ActionNode

#if WITH_EDITOR

URPG_DialogNodeBase* URPG_DialogObjectBase::CreateNode(ERPG_TypeDialogNode TypeNode, const FVector2D& NodePosition)
{
    const int32 FreeNumber = GetFreeIndexNumSlot();
    if (CLOG_DIALOG_SYSTEM(FreeNumber == INDEX_NONE, "Free number is INDEX_NONE")) return nullptr;

    URPG_DialogNodeBase* DialogNodeObject = nullptr;
    if (TypeNode == ERPG_TypeDialogNode::Start)
    {
        DialogNodeObject = CreateDefaultSubobject<URPG_DialogNodeStart>(FName("Start Dialog Node"));
    }
    else if (TypeNode == ERPG_TypeDialogNode::Work)
    {
        DialogNodeObject = NewObject<URPG_DialogNodeWork>(this, URPG_DialogNodeWork::StaticClass(), NAME_None, RF_Transactional);
    }
    else if (TypeNode == ERPG_TypeDialogNode::Finish)
    {
        DialogNodeObject = NewObject<URPG_DialogNodeFinish>(this, URPG_DialogNodeFinish::StaticClass(), NAME_None, RF_Transactional);
    }
    else if (TypeNode == ERPG_TypeDialogNode::Transfer)
    {
        DialogNodeObject = NewObject<URPG_DialogNodeTransfer>(this, URPG_DialogNodeTransfer::StaticClass(), NAME_None, RF_Transactional);
    }

    if (DialogNodeObject)
    {
        DialogNodeObject->SetFlags(RF_Transactional);
        DialogNodeObject->SetIndexNode(FreeNumber);
        DialogNodeObject->SetNodePosition(NodePosition);
        ArrayDialogNode.Add(DialogNodeObject);
    }

    return DialogNodeObject;
}

void URPG_DialogObjectBase::RemoveNode(const int32 IndexNode)
{
    URPG_DialogNodeBase* DialogNodeObject = FindNodeByIndex(IndexNode);
    if (DialogNodeObject && DialogNodeObject->GetTypeDialogNode() != ERPG_TypeDialogNode::Start)
    {
        ArrayDialogNode.Remove(DialogNodeObject);
        DialogNodeObject->ResetNode();
        DialogNodeObject->MarkAsGarbage();
        DialogNodeObject = nullptr;
    }
}

bool URPG_DialogObjectBase::Modify(bool bAlwaysMarkDirty)
{
    bool bSavedToTransactionBuffer = false;

    if (CanModify())
    {
        // Do not consider script packages, as they should never end up in the
        // transaction buffer and we don't want to mark them dirty here either.
        // We do want to consider PIE objects however
        if (GetOutermost()->HasAnyPackageFlags(PKG_ContainsScript | PKG_CompiledIn) == false || GetClass()->HasAnyClassFlags(CLASS_DefaultConfig | CLASS_Config))
        {
            // If we failed to save to the transaction buffer, but the user requested the package
            // marked dirty anyway, do so
            if (!bSavedToTransactionBuffer && bAlwaysMarkDirty)
            {
                MarkPackageDirty();
            }
        }

        FCoreUObjectDelegates::BroadcastOnObjectModified(this);
    }

    return bSavedToTransactionBuffer;
}

int32 URPG_DialogObjectBase::GetFreeIndexNumSlot() const
{
    for (int32 i = 0; i < INT32_MAX; i++)
    {
        if (!ArrayDialogNode.ContainsByPredicate([i](URPG_DialogNodeBase* Data) { return Data ? Data->GetIndexNode() == i : false; }))
        {
            return i;
        }
    }
    return INDEX_NONE;
}

#endif

#pragma endregion

bool URPG_DialogObjectBase::InitDialog(AActor* ActorOwner)
{
    if (!ActorOwner) return false;
    Owner = ActorOwner;

    const URPG_DialogNodeBase* StartNode = FindStartNode();
    if (!StartNode) return false;

    TargetIDNode = StartNode->GetIndexNode();
    ChangeDialogState(ERPG_DialogObjectState::Init);
    return true;
}

void URPG_DialogObjectBase::RunDialog()
{
    ChangeDialogState(ERPG_DialogObjectState::Run);
    NextDialog();
    UpdateTargetIDNode();
}

void URPG_DialogObjectBase::CompleteDialog()
{
    ChangeDialogState(ERPG_DialogObjectState::Finish);
    UpdateTargetIDNode();
}

void URPG_DialogObjectBase::NextDialog()
{
    if (CLOG_DIALOG_SYSTEM(GetWorld() == nullptr, "World is nullptr")) return;

    URPG_DialogNodeBase* FindNode = FindNodeByIndex(TargetIDNode);
    if (CLOG_DIALOG_SYSTEM(FindNode == nullptr, "Dialog node is nullptr")) return;
    int32 NextID = FindNode->GetNextIDNode();
    if (CLOG_DIALOG_SYSTEM(NextID == INDEX_NONE, "Next ID node is not valid")) return;

    URPG_DialogNodeBase* FindNextNode = FindNodeByIndex(NextID);
    if (CLOG_DIALOG_SYSTEM(FindNextNode == nullptr, "Next dialog node is nullptr")) return;

    if (FindNextNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Transfer)
    {
        TargetIDNode = FindNextNode->GetIndexNode();
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::NextDialog);
        return;
    }
    if (FindNextNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Finish)
    {
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CompleteDialog);
        return;
    }
    if (FindNextNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Start)
    {
        TargetIDNode = FindNextNode->GetIndexNode();
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::NextDialog);
        return;
    }
    if (FindNextNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        TargetIDNode = FindNextNode->GetIndexNode();
        FindNextNode->InitNode();
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::UpdateStateDialog);
    }
}

void URPG_DialogObjectBase::ResetDialog()
{
    TargetIDNode = INDEX_NONE;
    ChangeDialogState(ERPG_DialogObjectState::None);
}

void URPG_DialogObjectBase::UpdateTargetIDNode()
{
    if (CLOG_DIALOG_SYSTEM(GetWorld() == nullptr, "World is nullptr")) return;
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (CLOG_DIALOG_SYSTEM(PC == nullptr, "PC is nullptr")) return;
    AHUD* HUD = PC->GetHUD();
    if (CLOG_DIALOG_SYSTEM(HUD == nullptr, "HUD is nullptr")) return;

    if (HUD && HUD->Implements<UDialogActionInterface>())
    {
        AActor* LOwner = GetOwner();
        if (DialogState == ERPG_DialogObjectState::Run)
        {
            IDialogActionInterface::Execute_RunDialog(HUD, LOwner);
        }
        if (DialogState == ERPG_DialogObjectState::Finish)
        {
            IDialogActionInterface::Execute_CompleteDialog(HUD, LOwner);
        }
    }

    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::UpdateStateDialog);
}

void URPG_DialogObjectBase::ChangeDialogState(ERPG_DialogObjectState NewState)
{
    if (CLOG_DIALOG_SYSTEM(NewState == DialogState, "Dialog state is equal with new state : %s", *UEnum::GetValueAsString(NewState))) return;
    DialogState = NewState;
    LOG_DIALOG_SYSTEM(Display, "Dialog state changed: %s | Owner: %s", *UEnum::GetValueAsString(DialogState), *GetNameSafe(Owner.Get()));

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::UpdateStateDialog);
    }
}

#pragma endregion
