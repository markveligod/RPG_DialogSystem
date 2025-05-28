
#include "RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeWork.h"
#include "RPG_DialogSystemEditor/Settings/RPG_DialogSystemConfigEditor.h"
#include "Widgets/SDialogGraphNode.h"

#define PIN_DIALOG_IN TEXT("In")
#define PIN_DIALOG_OUT TEXT("Out")

URPG_DialogGraphNode_Base::URPG_DialogGraphNode_Base() {}

void URPG_DialogGraphNode_Base::InitNode(const URPG_DialogNodeBase* InObject)
{
    if (!InObject) return;
    TargetIndexNode = InObject->GetIndexNode();
    NodePosX = InObject->GetNodePosition().X;
    NodePosY = InObject->GetNodePosition().Y;
}

#pragma region EdGraphNodeInterface

#if WITH_EDITOR

bool URPG_DialogGraphNode_Base::Modify(bool bAlwaysMarkDirty)
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

#endif

void URPG_DialogGraphNode_Base::AllocateDefaultPins()
{
    Super::AllocateDefaultPins();

    URPG_DialogNodeBase* DialogNodeBase = GetOwnerNode();
    if (!DialogNodeBase) return;

    FName PinCategory = DialogNodeBase->GetFName();
    FEdGraphPinType PinType;
    PinType.PinCategory = PinCategory;
    PinType.PinSubCategory = TEXT("0");

    if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Start)
    {
        CreatePin(EGPD_Output, PinType, TEXT(""));
    }

    if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Finish)
    {
        CreatePin(EGPD_Input, PinType, TEXT(""));
    }

    if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Transfer)
    {
        CreatePin(EGPD_Input, PinType, TEXT(""));
    }

    if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        if (URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(DialogNodeBase))
        {
            CreatePin(EGPD_Input, PinType, TEXT(""));
            MakeWorkOutputPins(NodeWork);
        }
    }

    FCoreUObjectDelegates::OnObjectPropertyChanged.AddUObject(this, &ThisClass::DialogGraphNode_EditChangeProperty);
}

void URPG_DialogGraphNode_Base::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    Super::GetNodeContextMenuActions(Menu, Context);
}

FText URPG_DialogGraphNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    FText Title = Super::GetNodeTitle(TitleType);
    URPG_DialogNodeBase* DialogNodeBase = GetOwnerNode();
    if (!DialogNodeBase) return Title;

    return FText::FromString(FString::Printf(TEXT("%i | %s"), DialogNodeBase->GetIndexNode(), *GetNodeTitle(DialogNodeBase).ToString()));
}

FText URPG_DialogGraphNode_Base::GetTooltipText() const
{
    return Super::GetTooltipText();
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeTitleColor() const
{
    FLinearColor Color = Super::GetNodeBodyTintColor();
    URPG_DialogNodeBase* DialogNodeBase = GetOwnerNode();
    if (!DialogNodeBase) return Color;

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Color;

    if (DialogSystemConfigEditor->NodeTitleColors.Contains(DialogNodeBase->GetTypeDialogNode()))
    {
        Color = DialogSystemConfigEditor->NodeTitleColors[DialogNodeBase->GetTypeDialogNode()];
    }

    return Color;
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeCommentColor() const
{
    return Super::GetNodeCommentColor();
}

FLinearColor URPG_DialogGraphNode_Base::GetNodeBodyTintColor() const
{
    FLinearColor Color = Super::GetNodeBodyTintColor();
    URPG_DialogNodeBase* DialogNodeBase = GetOwnerNode();
    if (!DialogNodeBase) return Color;

    const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>();
    if (!DialogSystemConfigEditor) return Color;

    if (DialogSystemConfigEditor->NodeBodyTintColors.Contains(DialogNodeBase->GetTypeDialogNode()))
    {
        Color = DialogSystemConfigEditor->NodeBodyTintColors[DialogNodeBase->GetTypeDialogNode()];
    }

    return Color;
}

void URPG_DialogGraphNode_Base::NodeConnectionListChanged()
{
    Super::NodeConnectionListChanged();

    URPG_DialogNodeBase* DialogNode = GetOwnerNode();
    if (!DialogNode) return;

    if (DialogNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Start)
    {
        UEdGraphPin* MyPin = GetOutputPin();
        if (!MyPin) return;

        if (MyPin->LinkedTo.Num() > 0)
        {
            UEdGraphPin* LastLinkPin = MyPin->LinkedTo.Last();
            if (!LastLinkPin) return;
            URPG_DialogGraphNode_Base* OtherGraphNode = Cast<URPG_DialogGraphNode_Base>(LastLinkPin->GetOwningNode());
            if (!OtherGraphNode) return;
            int32 NextIndexNode = OtherGraphNode->GetTargetIndexNode();
            DialogNode->SetNextIDNode(NextIndexNode);
            MyPin->BreakAllPinLinks();
            LastLinkPin->BreakAllPinLinks();
            MyPin->MakeLinkTo(LastLinkPin);
        }
        else
        {
            DialogNode->SetNextIDNode(INDEX_NONE);
        }
    }
    else if (DialogNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(DialogNode);
        if (!NodeWork) return;
        TArray<UEdGraphPin*> OutPutPins = GetOutputPins();
        for (int32 PinIndex = 0; PinIndex < OutPutPins.Num(); ++PinIndex)
        {
            if (!OutPutPins.IsValidIndex(PinIndex)) continue;
            UEdGraphPin* MyPin = OutPutPins[PinIndex];
            if (!MyPin) continue;

            if (MyPin->LinkedTo.Num() > 0)
            {
                UEdGraphPin* LastLinkPin = MyPin->LinkedTo.Last();
                if (!LastLinkPin) return;
                URPG_DialogGraphNode_Base* OtherGraphNode = Cast<URPG_DialogGraphNode_Base>(LastLinkPin->GetOwningNode());
                if (!OtherGraphNode) return;
                int32 NextIndexNode = OtherGraphNode->GetTargetIndexNode();

                URPG_DialogPlayer* DialogPlayer = GetDialogPlayerByIndexPin(PinIndex);
                if (!DialogPlayer) continue;
                DialogPlayer->SetNextIDNode(NextIndexNode);

                MyPin->BreakAllPinLinks();
                LastLinkPin->BreakAllPinLinks();
                MyPin->MakeLinkTo(LastLinkPin);
            }
            else
            {
                URPG_DialogPlayer* DialogPlayer = GetDialogPlayerByIndexPin(PinIndex);
                if (!DialogPlayer) continue;
                DialogPlayer->SetNextIDNode(INDEX_NONE);
            }
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

URPG_DialogNodeBase* URPG_DialogGraphNode_Base::GetOwnerNode() const
{
    if (URPG_DialogObjectBase* Object = GetDialogObject())
    {
        return Object->FindNodeByIndex(TargetIndexNode);
    }
    return nullptr;
}

UEdGraphPin* URPG_DialogGraphNode_Base::GetInputPin() const
{
    for (auto* Pin : Pins)
    {
        if (!Pin) continue;
        if (Pin->Direction == EGPD_Input) return Pin;
    }
    return nullptr;
}

UEdGraphPin* URPG_DialogGraphNode_Base::GetOutputPin() const
{
    for (auto* Pin : Pins)
    {
        if (!Pin) continue;
        if (Pin->Direction == EGPD_Output) return Pin;
    }
    return nullptr;
}

void URPG_DialogGraphNode_Base::AutoConnectionPins(bool bMarkDirty) const
{
    URPG_DialogNodeBase* DialogNode = GetOwnerNode();
    if (!DialogNode) return;

    if (DialogNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(GetOwnerNode());
        if (!NodeWork) return;

        const TArray<URPG_DialogPlayer*>& ArrayDialogPlayers = NodeWork->GetOutputPinDialogs();
        for (int32 i = 0; i < ArrayDialogPlayers.Num(); i++)
        {
            if (!ArrayDialogPlayers.IsValidIndex(i)) continue;
            URPG_DialogPlayer* DialogPlayer = ArrayDialogPlayers[i];
            if (!DialogPlayer) continue;

            int32 IDNextNode = DialogPlayer->GetNextIDNode();
            if (IDNextNode == INDEX_NONE) continue;

            UEdGraphPin* MyPin = GetOutputPinByIndexWork(i);
            if (!MyPin) continue;

            URPG_DialogGraphNode_Base* OtherNode = FindGraphNode(IDNextNode);
            if (!OtherNode) continue;

            UEdGraphPin* OtherInputPin = OtherNode->GetInputPin();
            if (!OtherInputPin) continue;

            MyPin->MakeLinkTo(OtherInputPin, bMarkDirty);
        }
    }

    if (DialogNode->GetTypeDialogNode() == ERPG_TypeDialogNode::Start)
    {
        int32 IDNextNode = DialogNode->GetNextIDNode();
        if (IDNextNode == INDEX_NONE) return;

        UEdGraphPin* MyPin = GetOutputPin();
        if (!MyPin) return;

        URPG_DialogGraphNode_Base* OtherNode = FindGraphNode(IDNextNode);
        if (!OtherNode) return;

        UEdGraphPin* OtherInputPin = OtherNode->GetInputPin();
        if (!OtherInputPin) return;

        MyPin->MakeLinkTo(OtherInputPin, bMarkDirty);
    }
}

void URPG_DialogGraphNode_Base::ResetNode()
{
    BreakAllNodeLinks();
    RemoveOutputPins();
    if (UEdGraphPin* GraphPin = GetInputPin())
    {
        RemovePin(GraphPin);
    }
}

URPG_DialogGraphNode_Base* URPG_DialogGraphNode_Base::FindGraphNode(int32 TargetIndex) const
{
    UEdGraph* Graph = GetGraph();
    if (!Graph) return nullptr;
    for (auto& Node : Graph->Nodes)
    {
        URPG_DialogGraphNode_Base* DialogNode = Cast<URPG_DialogGraphNode_Base>(Node);
        if (!DialogNode) continue;
        if (DialogNode->GetTargetIndexNode() == TargetIndex) return DialogNode;
    }
    return nullptr;
}

URPG_DialogObjectBase* URPG_DialogGraphNode_Base::GetDialogObject() const
{
    return GetGraph() ? Cast<URPG_DialogObjectBase>(GetGraph()->GetOuter()) : nullptr;
}

UEdGraphPin* URPG_DialogGraphNode_Base::GetOutputPinByIndexWork(int32 IndexWork) const
{
    for (auto* Pin : Pins)
    {
        if (!Pin) continue;
        if (Pin->Direction == EGPD_Output)
        {
            int32 IDPin = FCString::Atoi(*Pin->PinType.PinSubCategory.ToString());
            if (IDPin != IndexWork) continue;
            return Pin;
        }
    }
    return nullptr;
}

URPG_DialogPlayer* URPG_DialogGraphNode_Base::GetDialogPlayerByIndexPin(int32 IndexPin) const
{
    URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(GetOwnerNode());
    if (!NodeWork) return nullptr;

    const TArray<URPG_DialogPlayer*>& ArrayPlayer = NodeWork->GetOutputPinDialogs();
    if (ArrayPlayer.IsValidIndex(IndexPin)) return ArrayPlayer[IndexPin];
    return nullptr;
}

void URPG_DialogGraphNode_Base::DialogGraphNode_EditChangeProperty(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
    URPG_DialogNodeBase* DialogNodeBase = GetOwnerNode();
    if (!DialogNodeBase) return;
    if (Object != GetOwnerNode()) return;

    DialogSGraphNode->UpdateOwnerEditorChange(PropertyChangedEvent);
    if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        RebuildOutputPins();
    }
}

void URPG_DialogGraphNode_Base::RebuildOutputPins()
{
    URPG_DialogNodeWork* NodeWork = Cast<URPG_DialogNodeWork>(GetOwnerNode());
    if (!NodeWork) return;

    BreakAllOutputNodeLinks();
    RemoveOutputPins();
    MakeWorkOutputPins(NodeWork);
    DialogSGraphNode->UpdateGraphNode();
    AutoConnectionPins();
}

void URPG_DialogGraphNode_Base::RemoveOutputPins()
{
    TArray<UEdGraphPin*> FindPins = GetOutputPins();
    for (int32 i = 0; i < FindPins.Num(); ++i)
    {
        if (!FindPins.IsValidIndex(i)) continue;
        RemovePin(FindPins[i]);
    }
}

TArray<UEdGraphPin*> URPG_DialogGraphNode_Base::GetOutputPins()
{
    TArray<UEdGraphPin*> OutputPins;
    for (UEdGraphPin* Pin : Pins)
    {
        if (!Pin) continue;
        if (Pin->Direction != EGPD_Output) continue;
        OutputPins.Add(Pin);
    }
    return OutputPins;
}

void URPG_DialogGraphNode_Base::BreakAllOutputNodeLinks(bool bAlwaysMarkDirty)
{
    for (int32 PinIdx = 0; PinIdx < Pins.Num(); ++PinIdx)
    {
        if (!Pins.IsValidIndex(PinIdx)) continue;
        UEdGraphPin* Pin = Pins[PinIdx];
        if (!Pin) continue;
        if (Pin->Direction != EGPD_Output) continue;

        constexpr bool bNotifyNodes = false;
        Pin->BreakAllPinLinks(bNotifyNodes, bAlwaysMarkDirty);
    }
}

FText URPG_DialogGraphNode_Base::GetNodeTitle(URPG_DialogNodeBase* Node) const
{
    if (!Node) return {};

    if (Node->GetTypeDialogNode() == ERPG_TypeDialogNode::Start)
    {
        return FText::FromString(TEXT("START"));
    }
    if (Node->GetTypeDialogNode() == ERPG_TypeDialogNode::Finish)
    {
        return FText::FromString(TEXT("FINISH"));
    }
    if (Node->GetTypeDialogNode() == ERPG_TypeDialogNode::Transfer)
    {
        return FText::FromString(TEXT("TRANSFER"));
    }
    if (Node->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        return FText::FromString(TEXT("WORK"));
    }

    return {};
}

void URPG_DialogGraphNode_Base::MakeWorkOutputPins(URPG_DialogNodeWork* NodeWork)
{
    FName PinCategory = NodeWork->GetFName();
    const TArray<URPG_DialogPlayer*>& ArrayDialogPlayers = NodeWork->GetOutputPinDialogs();
    for (int32 i = 0; i < ArrayDialogPlayers.Num(); i++)
    {
        if (!ArrayDialogPlayers.IsValidIndex(i)) continue;
        URPG_DialogPlayer* DialogPlayer = ArrayDialogPlayers[i];
        if (!DialogPlayer) continue;

        FEdGraphPinType PinType;
        PinType.PinCategory = PinCategory;
        PinType.PinSubCategory = FName(FString::Printf(TEXT("%i"), i));

        FString Text = DialogPlayer->GetText().ToString();
        if (Text.IsEmpty())
        {
            Text = TEXT("Empty");
        }
        FName PinName;
        if (Text.Len() > 25)
        {
            PinName = FName(FString::Printf(TEXT("%s..."), *Text.Left(25)));
        }
        else
        {
            PinName = FName(FString::Printf(TEXT("%s"), *Text));
        }
        CreatePin(EGPD_Output, PinType, PinName);
    }
}

#pragma endregion
