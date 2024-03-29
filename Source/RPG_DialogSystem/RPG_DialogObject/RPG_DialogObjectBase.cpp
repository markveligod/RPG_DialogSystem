// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "Condition/RPG_DialogSettingsObject.h"
#include "Net/UnrealNetwork.h"

#pragma region Default

URPG_DialogObjectBase::URPG_DialogObjectBase()
{
    if (HasAnyFlags(RF_ClassDefaultObject)) return;

#if UE_EDITOR

    if (!FindStartNode())
    {
        CreateNewDialogNode(ERPG_TypeStateDialog::Entry, FVector2D::ZeroVector);
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

bool URPG_DialogObjectBase::InitDialog(APlayerController* PlayerController)
{
    if (CLOG_DIALOG_SYSTEM(PlayerController == nullptr, "Player controller is nullptr")) return false;
    if (CLOG_DIALOG_SYSTEM(!PlayerController->HasAuthority(), "Player controller is not authority")) return false;
    OwnerPC = PlayerController;

    const URPG_DialogSettingsObject* DialogSettingsObject = FindStartNode();
    if (CLOG_DIALOG_SYSTEM(DialogSettingsObject == nullptr, "Start node is nullptr")) return false;
    TargetIDNode = DialogSettingsObject->IndexNode;

    return true;
}

void URPG_DialogObjectBase::NextDialog()
{
    URPG_DialogSettingsObject* TargetNode = FindNodeByIndex(TargetIDNode);
    if (CLOG_DIALOG_SYSTEM(TargetNode == nullptr, "Target node is nullptr")) return;
    TargetNode->ExecuteEvents(OwnerPC);

    TargetNode->OutNodes.Sort([&](int32 LeftID, int32 RightID)
    {
        const URPG_DialogSettingsObject* LeftDialogSettings = FindNodeByIndex(LeftID);
        const URPG_DialogSettingsObject* RightDialogSettings = FindNodeByIndex(RightID);
        if (LeftDialogSettings && RightDialogSettings)
        {
            return LeftDialogSettings->NodePosition.Y < RightDialogSettings->NodePosition.Y;
        }
        return false;
    });

    const int32* FindID = TargetNode->OutNodes.FindByPredicate([&](int32 IDNode)
    {
        const URPG_DialogSettingsObject* DialogSettings = FindNodeByIndex(IDNode);
        return DialogSettings && DialogSettings->IsValidCondition(OwnerPC);
    });

    PushNextNodeDialog(FindID ? *FindID : INDEX_NONE);
}

void URPG_DialogObjectBase::ResetDialog()
{
    TargetIDNode = INDEX_NONE;
    OwnerPC = nullptr;
}

URPG_DialogSettingsObject* URPG_DialogObjectBase::FindNodeByIndex(int32 IndexNode)
{
    const auto FindElem = ArrayDialogNode.FindByPredicate([IndexNode](URPG_DialogSettingsObject* Data)
    {
        return Data ? Data->IndexNode == IndexNode : false;
    });
    return FindElem ? *FindElem : nullptr;
}

URPG_DialogSettingsObject* URPG_DialogObjectBase::FindStartNode()
{
    const auto FindElem = ArrayDialogNode.FindByPredicate([](URPG_DialogSettingsObject* Data)
    {
        return Data ? Data->TypeStateDialog == ERPG_TypeStateDialog::Entry : false;
    });
    return FindElem ? *FindElem : nullptr;
}

TArray<URPG_DialogSettingsObject*> URPG_DialogObjectBase::FindValidPlayerNodes(int32 IndexNode)
{
    TArray<URPG_DialogSettingsObject*> ArrayResult;
    if (IsSomeHaveOutPlayerNode({IndexNode}))
    {
        if (URPG_DialogSettingsObject* DialogSettings = FindNodeByIndex(IndexNode))
        {
            for (int32 Node : DialogSettings->OutNodes)
            {
                if (!IsPlayerNode(Node)) continue;
                URPG_DialogSettingsObject* CheckDialogSettings = FindNodeByIndex(Node);
                if (!CheckDialogSettings) continue;
                if (!CheckDialogSettings->IsValidCondition(OwnerPC)) continue;
                ArrayResult.Add(CheckDialogSettings);
            }
        }
    }

    return ArrayResult;
}

bool URPG_DialogObjectBase::IsPlayerNode(const int32 IndexNode)
{
    if (const URPG_DialogSettingsObject* DialogSettings = FindNodeByIndex(IndexNode))
    {
        return DialogSettings->TypeStateDialog == ERPG_TypeStateDialog::PlayerNode;
    }
    return false;
}

bool URPG_DialogObjectBase::IsSomeHaveOutPlayerNode(const TArray<int32>& OutNodes)
{
    for (const int32 NodeID : OutNodes)
    {
        const URPG_DialogSettingsObject* DialogSetting = FindNodeByIndex(NodeID);
        if (!DialogSetting) continue;
        if (DialogSetting->TypeStateDialog == ERPG_TypeStateDialog::PlayerNode)
        {
            return true;
        }
    }
    return false;
}

void URPG_DialogObjectBase::RemoveIndexNode(int32 IndexNode)
{
#if UE_EDITOR
    URPG_DialogSettingsObject* DialogSettingsObject = FindNodeByIndex(IndexNode);
    if (!DialogSettingsObject) return;

    for (auto Node : ArrayDialogNode)
    {
        Node->OutNodes.Remove(IndexNode);
    }

    ArrayDialogNode.Remove(DialogSettingsObject);
    DialogSettingsObject->MarkAsGarbage();
#endif
}

void URPG_DialogObjectBase::ServerNextDialog_Implementation()
{
    NextDialog();
}

bool URPG_DialogObjectBase::ServerNextDialog_Validate()
{
    return true;
}

void URPG_DialogObjectBase::ServerPushNextNodeDialog_Implementation(int32 IDNode)
{
    
}

bool URPG_DialogObjectBase::ServerPushNextNodeDialog_Validate(int32 IDNode)
{
    return true;
}

void URPG_DialogObjectBase::OnRep_TargetIDNode()
{
    OnNextNodeDialog.Broadcast(FindNodeByIndex(TargetIDNode));
}

void URPG_DialogObjectBase::PushNextNodeDialog(int32 IDNode)
{
    if (CLOG_DIALOG_SYSTEM(OwnerPC == nullptr, "Player controller is nullptr")) return;

    if (OwnerPC->HasAuthority())
    {
        TargetIDNode = IDNode;
        OnNextNodeDialog.Broadcast(FindNodeByIndex(TargetIDNode));
        if (IDNode == INDEX_NONE)
        {
            GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CompleteDialog);
        }
    }
    else
    {
        ServerPushNextNodeDialog(IDNode);
    }
}

void URPG_DialogObjectBase::CompleteDialog()
{
    OnCompleteDialog.ExecuteIfBound(this);
}

#if WITH_EDITOR

URPG_DialogSettingsObject* URPG_DialogObjectBase::CreateNewDialogNode(const ERPG_TypeStateDialog& TypeStateDialog, FVector2D NodePosition)
{
    const int32 FreeNumber = GetFreeIndexNumSlot();
    if (CLOG_DIALOG_SYSTEM(FreeNumber == INDEX_NONE, "Free number is INDEX_NONE")) return nullptr;

    URPG_DialogSettingsObject* NewDialogSettingsObject = nullptr;
    if (TypeStateDialog == ERPG_TypeStateDialog::Entry)
    {
        NewDialogSettingsObject = CreateDefaultSubobject<URPG_DialogSettingsObject>(FName("Start Dialog Node"));
    }
    else
    {
        NewDialogSettingsObject = NewObject<URPG_DialogSettingsObject>(this, URPG_DialogSettingsObject::StaticClass(), NAME_None, RF_Transactional);
    }

    if (NewDialogSettingsObject)
    {
        NewDialogSettingsObject->SetFlags(RF_Transactional);
        NewDialogSettingsObject->IndexNode = FreeNumber;
        NewDialogSettingsObject->NodePosition = NodePosition;
        NewDialogSettingsObject->TypeStateDialog = TypeStateDialog;
        NewDialogSettingsObject->SetupNewTextDialog(FText::FromString(FString::Printf(TEXT("Example Text #%i"), FreeNumber)));
        ArrayDialogNode.AddUnique(NewDialogSettingsObject);
        return NewDialogSettingsObject;
    }

    return nullptr;
}

int32 URPG_DialogObjectBase::GetFreeIndexNumSlot() const
{
    for (int32 i = 0; i < INT32_MAX; i++)
    {
        if (!ArrayDialogNode.ContainsByPredicate([i](URPG_DialogSettingsObject* Data)
        {
            return Data ? Data->IndexNode == i : false;
        }))
        {
            return i;
        }
    }
    return INDEX_NONE;
}

#endif

#pragma endregion
