// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogGraph.h"
#include "Nodes/RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "UObject/ObjectSaveContext.h"

URPG_DialogObjectBase* URPG_DialogGraph::GetDialogObject() const
{
    return CastChecked<URPG_DialogObjectBase>(GetOuter());
}

void URPG_DialogGraph::PreSave(FObjectPreSaveContext SaveContext)
{
    for (auto& Node : Nodes)
    {
        if (URPG_DialogGraphNode_Base* DialogGraphNode = Cast<URPG_DialogGraphNode_Base>(Node.Get()))
        {
            if (URPG_DialogNodeBase* DialogNodeBase = DialogGraphNode->GetOwnerNode())
            {
                DialogNodeBase->SetNodePosition(FVector2D(DialogGraphNode->NodePosX, DialogGraphNode->NodePosY));
            }
        }
    }

    Super::PreSave(SaveContext);
}

UEdGraphNode* URPG_DialogGraph::CreateNode(FVector2D Pos, ERPG_TypeDialogNode TypeNode)
{
    URPG_DialogObjectBase* DialogObject = Cast<URPG_DialogObjectBase>(GetOuter());
    if (!DialogObject) return nullptr;

    if (URPG_DialogNodeBase* NewNode = DialogObject->CreateNode(TypeNode, Pos))
    {
        FGraphNodeCreator<URPG_DialogGraphNode_Base> NodeCreator(*this);
        URPG_DialogGraphNode_Base* ResultRootNode = NodeCreator.CreateNode();
        if (!ResultRootNode) return nullptr;

        ResultRootNode->InitNode(NewNode);
        NodeCreator.Finalize();
        GetSchema()->SetNodeMetaData(ResultRootNode, FNodeMetadata::DefaultGraphNode);
        ResultRootNode->Modify();
        ResultRootNode->MarkPackageDirty();
        return ResultRootNode;
    }
    return nullptr;
}
