// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogEdGraphSchema.h"
#include "Actions/RPG_DialogGraphActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "Nodes/RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
#include "RPG_DialogSystemEditor/Settings/RPG_DialogSystemConfigEditor.h"

#define DISALLOW_CONNECT FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Not implemented by this schema"))
#define ALLOW_CONNECT FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Allow connect pins"));

#define LOCTEXT_NAMESPACE "DialogSchema"

void URPG_DialogEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    // For action graph
    Super::GetGraphContextActions(ContextMenuBuilder);

    const TSharedPtr<FRPG_DialogAction_NewNPCNode> CreateNPCAction(new FRPG_DialogAction_NewNPCNode());
    ContextMenuBuilder.AddAction(CreateNPCAction);

    const TSharedPtr<FRPG_DialogAction_NewPlayerNode> CreatePlayerAction(new FRPG_DialogAction_NewPlayerNode());
    ContextMenuBuilder.AddAction(CreatePlayerAction);

    const TSharedPtr<FRPG_DialogAction_NewTransferNode> CreateTransferAction(new FRPG_DialogAction_NewTransferNode());
    ContextMenuBuilder.AddAction(CreateTransferAction);
}

void URPG_DialogEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    // For Node
    Super::GetContextMenuActions(Menu, Context);

    if (Context && Context->Node && Context->Node->IsA(URPG_DialogGraphNode_Base::StaticClass()))
    {
        FToolMenuSection& Section = Menu->AddSection(FName("DialogGraphSchemaNodeActions"), FText::FromString(TEXT("Node Actions")));
        Section.AddMenuEntry(FGenericCommands::Get().Delete);
    }
}

void URPG_DialogEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
    URPG_DialogObjectBase* DialogObject = Cast<URPG_DialogObjectBase>(Graph.GetOuter());
    if (!DialogObject) return;

    const TArray<URPG_DialogSettingsObject*>& ArrayDialogNode = DialogObject->GetArrayDialogNode();
    for (const auto* NodeData : ArrayDialogNode)
    {
        FGraphNodeCreator<URPG_DialogGraphNode_Base> NodeCreator(Graph);
        URPG_DialogGraphNode_Base* ResultRootNode = NodeCreator.CreateNode();
        if (!ResultRootNode) continue;

        ResultRootNode->TargetIndexTaskNode = NodeData->IndexNode;
        ResultRootNode->NodePosX = NodeData->NodePosition.X;
        ResultRootNode->NodePosY = NodeData->NodePosition.Y;
        NodeCreator.Finalize();
        SetNodeMetaData(ResultRootNode, FNodeMetadata::DefaultGraphNode);
        ResultRootNode->Modify();
        ResultRootNode->MarkPackageDirty();
    }

    AutoConnectNodeByDefault(Graph);
}

void URPG_DialogEdGraphSchema::AutoConnectNodeByDefault(UEdGraph& Graph) const
{
    for (auto Node : Graph.Nodes)
    {
        const URPG_DialogGraphNode_Base* DialogGraphNode = Cast<URPG_DialogGraphNode_Base>(Node.Get());
        if (!DialogGraphNode) continue;

        URPG_DialogSettingsObject* DialogSettingData = DialogGraphNode->GetDialogSettingsObject();
        if (!DialogSettingData) continue;

        for (const int32 IndexNode : DialogSettingData->OutNodes)
        {
            DialogGraphNode->MakeLink(FindDialogGraphNodeByIndex(Graph, IndexNode));
        }
    }
}

const FPinConnectionResponse URPG_DialogEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    if (!A || !B) return DISALLOW_CONNECT;
    if (A == B) return DISALLOW_CONNECT;
    if (A->Direction == EGPD_Input && B->Direction == EGPD_Input) return DISALLOW_CONNECT;
    if (A->Direction == EGPD_Output && B->Direction == EGPD_Output) return DISALLOW_CONNECT;
    if (A->GetOwningNode() == B->GetOwningNode()) return DISALLOW_CONNECT;

    URPG_DialogGraphNode_Base* A_GraphNode = Cast<URPG_DialogGraphNode_Base>(A->GetOwningNode());
    URPG_DialogGraphNode_Base* B_GraphNode = Cast<URPG_DialogGraphNode_Base>(B->GetOwningNode());
    if (!A_GraphNode || !B_GraphNode) return DISALLOW_CONNECT;

    URPG_DialogSettingsObject* A_DialogSettingsObject = A_GraphNode->GetDialogSettingsObject();
    URPG_DialogSettingsObject* B_DialogSettingsObject = B_GraphNode->GetDialogSettingsObject();
    if (!A_DialogSettingsObject || !B_DialogSettingsObject) return DISALLOW_CONNECT;
    if (A_DialogSettingsObject->TypeStateDialog == ERPG_TypeStateDialog::PlayerNode && B_DialogSettingsObject->TypeStateDialog == ERPG_TypeStateDialog::PlayerNode) return DISALLOW_CONNECT;

    return ALLOW_CONNECT;
}

bool URPG_DialogEdGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
    if (Super::TryCreateConnection(A, B))
    {
        URPG_DialogGraphNode_Base* A_GraphNode = Cast<URPG_DialogGraphNode_Base>(A->GetOwningNode());
        URPG_DialogGraphNode_Base* B_GraphNode = Cast<URPG_DialogGraphNode_Base>(B->GetOwningNode());
        if (!A_GraphNode || !B_GraphNode) return false;

        A_GraphNode->NodeConnectionListChanged();
        B_GraphNode->NodeConnectionListChanged();
    }

    return false;
}

FLinearColor URPG_DialogEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
    if (const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>())
    {
        return DialogSystemConfigEditor->SchemeTypePinColor;
    }
    return Super::GetPinTypeColor(PinType);
}

FLinearColor URPG_DialogEdGraphSchema::GetSecondaryPinTypeColor(const FEdGraphPinType& PinType) const
{
    if (const URPG_DialogSystemConfigEditor* DialogSystemConfigEditor = GetDefault<URPG_DialogSystemConfigEditor>())
    {
        return DialogSystemConfigEditor->SchemeSecondaryPinColor;
    }
    return Super::GetSecondaryPinTypeColor(PinType);
}

UEdGraphNode* URPG_DialogEdGraphSchema::CreateStandardNodeForGraph(UEdGraph* Graph, const FVector2D& InLocationNode, ERPG_TypeStateDialog TypeStateDialog) const
{
    if (!Graph) return nullptr;
    URPG_DialogObjectBase* DialogObject = Cast<URPG_DialogObjectBase>(Graph->GetOuter());
    if (!DialogObject) return nullptr;

    if (URPG_DialogSettingsObject* NewDialog = DialogObject->CreateNewDialogNode(TypeStateDialog, InLocationNode))
    {
        FGraphNodeCreator<URPG_DialogGraphNode_Base> NodeCreator(*Graph);
        URPG_DialogGraphNode_Base* ResultRootNode = NodeCreator.CreateNode();
        if (!ResultRootNode) return nullptr;

        ResultRootNode->TargetIndexTaskNode = NewDialog->IndexNode;
        ResultRootNode->NodePosX = NewDialog->NodePosition.X;
        ResultRootNode->NodePosY = NewDialog->NodePosition.Y;
        NodeCreator.Finalize();
        SetNodeMetaData(ResultRootNode, FNodeMetadata::DefaultGraphNode);
        ResultRootNode->Modify();
        ResultRootNode->MarkPackageDirty();
        return ResultRootNode;
    }
    return nullptr;
}

URPG_DialogGraphNode_Base* URPG_DialogEdGraphSchema::FindDialogGraphNodeByIndex(UEdGraph& Graph, int32 TargetIndexNode) const
{
    const auto FindElem = Graph.Nodes.FindByPredicate([TargetIndexNode](TObjectPtr<UEdGraphNode>& Data)
    {
        URPG_DialogGraphNode_Base* DialogGraphNode = Cast<URPG_DialogGraphNode_Base>(Data.Get());
        if (!DialogGraphNode) return false;
        return DialogGraphNode->TargetIndexTaskNode == TargetIndexNode;
    });

    return FindElem ? Cast<URPG_DialogGraphNode_Base>(FindElem->Get()) : nullptr;
}

#undef LOCTEXT_NAMESPACE
