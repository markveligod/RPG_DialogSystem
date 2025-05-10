// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "Net/UnrealNetwork.h"
#include "Nodes/RPG_DialogNodeFinish.h"
#include "Nodes/RPG_DialogNodeStart.h"
#include "Nodes/RPG_DialogNodeTransfer.h"
#include "Nodes/RPG_DialogNodeWork.h"

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

#pragma endregion

#pragma region NetworkInterface

int32 URPG_DialogObjectBase::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
    if (!OwnerPC && GetWorld()->GetNetMode() == NM_Client)
    {
        OwnerPC = GetWorld()->GetFirstPlayerController();
    }
    return (OwnerPC ? OwnerPC->GetFunctionCallspace(Function, Stack) : FunctionCallspace::Local);
}

bool URPG_DialogObjectBase::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
    if (!OwnerPC && GetWorld()->GetNetMode() == NM_Client)
    {
        OwnerPC = GetWorld()->GetFirstPlayerController();
    }
    if (!OwnerPC) return false;
    UNetDriver* NetDriver = OwnerPC->GetNetDriver();
    if (!NetDriver) return false;
    NetDriver->ProcessRemoteFunction(OwnerPC, Function, Parms, OutParms, Stack, this);
    return true;
}

void URPG_DialogObjectBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(URPG_DialogObjectBase, ArrayDialogNode, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(URPG_DialogObjectBase, TargetIDNode, COND_OwnerOnly);
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

bool URPG_DialogObjectBase::InitDialog(APlayerController* PlayerController)
{
    return true;
}

void URPG_DialogObjectBase::NextDialog() {}

void URPG_DialogObjectBase::ResetDialog()
{
    TargetIDNode = INDEX_NONE;
    OwnerPC = nullptr;
}

bool URPG_DialogObjectBase::IsSomeHaveOutPlayerNode(const TArray<int32>& OutNodes)
{
    return false;
}

void URPG_DialogObjectBase::RemoveIndexNode(int32 IndexNode) {}

void URPG_DialogObjectBase::OnRep_TargetIDNode() {}

#pragma endregion
