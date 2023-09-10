﻿#include "RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
#include "RPG_DialogSystemEditor/Settings/RPG_DialogSystemConfigEditor.h"

URPG_DialogGraphNode_Base::URPG_DialogGraphNode_Base()
{
}

#pragma region EdGraphNodeInterface

void URPG_DialogGraphNode_Base::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();
}

void URPG_DialogGraphNode_Base::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    Super::GetNodeContextMenuActions(Menu, Context);
}

FText URPG_DialogGraphNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    URPG_DialogObjectBase* DialogObject = GetDialogObject();
    if (!DialogObject) return Super::GetNodeTitle(TitleType);

    const URPG_DialogSettingsObject* DialogSettingsObject = DialogObject->FindNodeByIndex(TargetIndexTaskNode);
    if (!DialogSettingsObject) return Super::GetNodeTitle(TitleType);

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetNodeTitle(TitleType);

    const FString NodeTitleText = DialogSystemConfigEditor->NodeTitleText.Contains(DialogSettingsObject->TypeStateDialog) ?
        DialogSystemConfigEditor->NodeTitleText[DialogSettingsObject->TypeStateDialog] : TEXT("None");

    return FText::FromString(FString::Printf(TEXT("#%i | %s"),
        DialogSettingsObject->IndexNode, *NodeTitleText));
}

FText URPG_DialogGraphNode_Base::GetTooltipText() const
{
    URPG_DialogObjectBase* DialogObject = GetDialogObject();
    if (!DialogObject) return Super::GetTooltipText();

    const URPG_DialogSettingsObject* DialogSettingsObject = DialogObject->FindNodeByIndex(TargetIndexTaskNode);
    if (!DialogSettingsObject) return Super::GetTooltipText();

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetTooltipText();

    const FString NodeTooltipText = DialogSystemConfigEditor->NodeTooltipText.Contains(DialogSettingsObject->TypeStateDialog) ?
        DialogSystemConfigEditor->NodeTooltipText[DialogSettingsObject->TypeStateDialog] : TEXT("None");

    return FText::FromString(FString::Printf(TEXT("#%i | %s"),
        DialogSettingsObject->IndexNode, *NodeTooltipText));
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeTitleColor() const
{
    URPG_DialogObjectBase* DialogObject = GetDialogObject();
    if (!DialogObject) return Super::GetNodeTitleColor();
    URPG_DialogSettingsObject* DialogSettingsObject = DialogObject->FindNodeByIndex(TargetIndexTaskNode);
    if (!DialogSettingsObject) return Super::GetNodeTitleColor();

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetNodeTitleColor();

    if (DialogSystemConfigEditor->NodeTitleColor.Contains(DialogSettingsObject->TypeStateDialog))
    {
        return DialogSystemConfigEditor->NodeTitleColor[DialogSettingsObject->TypeStateDialog];
    }
    
    return Super::GetNodeTitleColor();
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeCommentColor() const
{
    URPG_DialogObjectBase* DialogObject = GetDialogObject();
    if (!DialogObject) return Super::GetNodeCommentColor();
    URPG_DialogSettingsObject* DialogSettingsObject = DialogObject->FindNodeByIndex(TargetIndexTaskNode);
    if (!DialogSettingsObject) return Super::GetNodeCommentColor();

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetNodeCommentColor();

    if (DialogSystemConfigEditor->NodeCommentColor.Contains(DialogSettingsObject->TypeStateDialog))
    {
        return DialogSystemConfigEditor->NodeCommentColor[DialogSettingsObject->TypeStateDialog];
    }
    
    return Super::GetNodeCommentColor();
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeBodyTintColor() const
{
    URPG_DialogObjectBase* DialogObject = GetDialogObject();
    if (!DialogObject) return Super::GetNodeBodyTintColor();
    URPG_DialogSettingsObject* DialogSettingsObject = DialogObject->FindNodeByIndex(TargetIndexTaskNode);
    if (!DialogSettingsObject) return Super::GetNodeBodyTintColor();

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetNodeBodyTintColor();

    if (DialogSystemConfigEditor->NodeBodyTintColor.Contains(DialogSettingsObject->TypeStateDialog))
    {
        return DialogSystemConfigEditor->NodeBodyTintColor[DialogSettingsObject->TypeStateDialog];
    }
    
    return Super::GetNodeBodyTintColor();
}

void URPG_DialogGraphNode_Base::NodeConnectionListChanged()
{
    Super::NodeConnectionListChanged();
}

#pragma endregion

#pragma region ActionNode

URPG_DialogObjectBase* URPG_DialogGraphNode_Base::GetDialogObject() const
{
    return GetGraph() ? Cast<URPG_DialogObjectBase>(GetGraph()->GetOuter()) : nullptr;
}

#pragma endregion