// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogObject/RPG_DialogObjectBase.h"
#include "Condition/RPG_DialogSettingsObject.h"
#include "Net/UnrealNetwork.h"

#pragma region Default

bool URPG_DialogObjectBase::InitDialog(APlayerController* PlayerController)
{
    if (CLOG_DIALOG_SYSTEM(PlayerController == nullptr, "Player controller is nullptr")) return false;

    OwnerPC = PlayerController;
    return true;
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
}

#pragma endregion

#pragma region Action

FRPG_DialogNode* URPG_DialogObjectBase::FindNodeByIndex(int32 IndexNode)
{
    return ArrayDialogNode.FindByPredicate([IndexNode](const FRPG_DialogNode& Data)
    {
        return Data.IndexNode == IndexNode;
    });
}

FRPG_DialogNode* URPG_DialogObjectBase::FindStartNode()
{
    return ArrayDialogNode.FindByPredicate([](const FRPG_DialogNode& Data)
    {
        return Data.TypeStateDialog == ERPG_TypeStateDialog::Entry;
    });
}

FRPG_DialogNode* URPG_DialogObjectBase::CreateNewDialogNode(const ERPG_TypeStateDialog& TypeStateDialog, FVector2D NodePosition)
{
    const int32 FreeNumber = GetFreeIndexNumSlot();
    if (CLOG_DIALOG_SYSTEM(FreeNumber == INDEX_NONE, "Free number is INDEX_NONE")) return nullptr;
    FRPG_DialogNode NewNode;

    NewNode.IndexNode = FreeNumber;
    NewNode.NodePosition = NodePosition;
    NewNode.TypeStateDialog = TypeStateDialog;
    NewNode.DialogConditionObject = NewObject<URPG_DialogSettingsObject>(this, URPG_DialogSettingsObject::StaticClass(), NAME_None, RF_Transactional);
    if (NewNode.DialogConditionObject)
    {
        NewNode.DialogConditionObject->SetupNewTextDialog(FText::FromString(FString::Printf(TEXT("Example Text #%i"), FreeNumber)));
    }

    ArrayDialogNode.AddUnique(NewNode);
    return FindNodeByIndex(FreeNumber);
}

int32 URPG_DialogObjectBase::GetFreeIndexNumSlot() const
{
    for (int32 i = 0; i < INT32_MAX; i++)
    {
        if (!ArrayDialogNode.ContainsByPredicate([i](const FRPG_DialogNode& Data) { return Data.IndexNode == i; }))
        {
            return i;
        }
    }
    return INDEX_NONE;
}

#pragma endregion
