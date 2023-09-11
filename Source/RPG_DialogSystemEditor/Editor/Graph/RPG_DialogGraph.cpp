// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogGraph.h"
#include "Nodes/RPG_DialogGraphNode_Base.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
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
            if (URPG_DialogSettingsObject* DialogSettingsObject = DialogGraphNode->GetDialogSettingsObject())
            {
                DialogSettingsObject->NodePosition = FVector2D(DialogGraphNode->NodePosX, DialogGraphNode->NodePosY);
            }
        }
    }

    Super::PreSave(SaveContext);
}
