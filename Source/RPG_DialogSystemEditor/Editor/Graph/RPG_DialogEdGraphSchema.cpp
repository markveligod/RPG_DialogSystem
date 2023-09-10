// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogEdGraphSchema.h"
#include "Actions/RPG_DialogGraphActions.h"
#include "Nodes/RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"

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
}

const FPinConnectionResponse URPG_DialogEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    return Super::CanCreateConnection(A, B);
}

bool URPG_DialogEdGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
    return Super::TryCreateConnection(A, B);
}

FLinearColor URPG_DialogEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
    return Super::GetPinTypeColor(PinType);
}

FLinearColor URPG_DialogEdGraphSchema::GetSecondaryPinTypeColor(const FEdGraphPinType& PinType) const
{
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

#undef LOCTEXT_NAMESPACE
