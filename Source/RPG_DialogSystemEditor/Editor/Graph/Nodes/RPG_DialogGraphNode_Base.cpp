#include "RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
#include "RPG_DialogSystemEditor/Settings/RPG_DialogSystemConfigEditor.h"
#include "Widgets/SDialogGraphNode.h"

#define PIN_DIALOG_IN TEXT("In")
#define PIN_DIALOG_OUT TEXT("Out")

URPG_DialogGraphNode_Base::URPG_DialogGraphNode_Base()
{
}

#pragma region EdGraphNodeInterface

void URPG_DialogGraphNode_Base::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();

    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
    if (!DialogSettingsObject) return;

    FName PinCategory = DialogSettingsObject->GetFName();

    if (DialogSettingsObject->TypeStateDialog == ERPG_TypeStateDialog::Entry)
    {
        OutputPin = CreatePin(EGPD_Output, PinCategory, TEXT("Entry"), PIN_DIALOG_OUT);
    }

    if (DialogSettingsObject->TypeStateDialog == ERPG_TypeStateDialog::PlayerNode || DialogSettingsObject->TypeStateDialog == ERPG_TypeStateDialog::NPCNode)
    {
        InPin = CreatePin(EGPD_Input, PinCategory, *UEnum::GetValueAsString(DialogSettingsObject->TypeStateDialog), PIN_DIALOG_IN);
        OutputPin = CreatePin(EGPD_Output, PinCategory, *UEnum::GetValueAsString(DialogSettingsObject->TypeStateDialog), PIN_DIALOG_OUT);
    }

    if (DialogSettingsObject->TypeStateDialog == ERPG_TypeStateDialog::Transfer)
    {
        InPin = CreatePin(EGPD_Input, PinCategory, TEXT("Transfer"), PIN_DIALOG_IN);
    }

    FCoreUObjectDelegates::OnObjectPropertyChanged.AddUObject(this, &ThisClass::DialogGraphNode_EditChangeProperty);
}

void URPG_DialogGraphNode_Base::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    Super::GetNodeContextMenuActions(Menu, Context);
}

FText URPG_DialogGraphNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
    if (!DialogSettingsObject) return Super::GetNodeTitle(TitleType);

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetNodeTitle(TitleType);

    const FString NodeTitleText = DialogSystemConfigEditor->NodeTitleText.Contains(DialogSettingsObject->TypeStateDialog)
                                      ? DialogSystemConfigEditor->NodeTitleText[DialogSettingsObject->TypeStateDialog]
                                      : TEXT("None");

    return FText::FromString(FString::Printf(TEXT("#%i | %s"),
        DialogSettingsObject->IndexNode, *NodeTitleText));
}

FText URPG_DialogGraphNode_Base::GetTooltipText() const
{
    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
    if (!DialogSettingsObject) return Super::GetTooltipText();

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Super::GetTooltipText();

    const FString NodeTooltipText = DialogSystemConfigEditor->NodeTooltipText.Contains(DialogSettingsObject->TypeStateDialog)
                                        ? DialogSystemConfigEditor->NodeTooltipText[DialogSettingsObject->TypeStateDialog]
                                        : TEXT("None");

    return FText::FromString(FString::Printf(TEXT("#%i | %s"),
        DialogSettingsObject->IndexNode, *NodeTooltipText));
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeTitleColor() const
{
    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
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
    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
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
    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
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

    URPG_DialogSettingsObject* DialogSettingsObject = GetDialogSettingsObject();
    if (!DialogSettingsObject) return;

    if (!OutputPin) return;
    for (UEdGraphPin* Pin : OutputPin->LinkedTo)
    {
        if (!Pin) continue;
        if (const URPG_DialogGraphNode_Base* DialogGraphNode = Cast<URPG_DialogGraphNode_Base>(Pin->GetOwningNode()))
        {
            DialogSettingsObject->OutNodes.AddUnique(DialogGraphNode->TargetIndexTaskNode);
        }
    }
}

TSharedPtr<SGraphNode> URPG_DialogGraphNode_Base::CreateVisualWidget()
{
    DialogSGraphNode = SNew(SDialogGraphNode, this);
    return DialogSGraphNode;
}

#pragma endregion

#pragma region ActionNode

URPG_DialogSettingsObject* URPG_DialogGraphNode_Base::GetDialogSettingsObject() const
{
    URPG_DialogObjectBase* DialogObject = GetDialogObject();
    if (!DialogObject) return nullptr;

    return DialogObject->FindNodeByIndex(TargetIndexTaskNode);
}

void URPG_DialogGraphNode_Base::MakeLink(const URPG_DialogGraphNode_Base* To) const
{
    if (OutputPin && To)
    {
        OutputPin->MakeLinkTo(To->InPin);
    }
}

URPG_DialogObjectBase* URPG_DialogGraphNode_Base::GetDialogObject() const
{
    return GetGraph() ? Cast<URPG_DialogObjectBase>(GetGraph()->GetOuter()) : nullptr;
}

void URPG_DialogGraphNode_Base::DialogGraphNode_EditChangeProperty(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
    if (Object != GetDialogSettingsObject()) return;

    DialogSGraphNode->UpdateOwnerEditorChange(PropertyChangedEvent);
}

#pragma endregion
