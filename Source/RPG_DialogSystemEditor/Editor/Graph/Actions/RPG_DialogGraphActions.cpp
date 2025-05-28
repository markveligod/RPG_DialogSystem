#include "RPG_DialogGraphActions.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogSystemEditor/Editor/Graph/RPG_DialogGraph.h"

UEdGraphNode* FRPG_DialogAction_NewWorkNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    if (URPG_DialogGraph* DialogGraph = Cast<URPG_DialogGraph>(ParentGraph))
    {
        return DialogGraph->CreateNode(Location, ERPG_TypeDialogNode::Work);
    }
    return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}

UEdGraphNode* FRPG_DialogAction_NewFinishNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    if (URPG_DialogGraph* DialogGraph = Cast<URPG_DialogGraph>(ParentGraph))
    {
        return DialogGraph->CreateNode(Location, ERPG_TypeDialogNode::Finish);
    }
    return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}

UEdGraphNode* FRPG_DialogAction_NewTransferNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    if (URPG_DialogGraph* DialogGraph = Cast<URPG_DialogGraph>(ParentGraph))
    {
        return DialogGraph->CreateNode(Location, ERPG_TypeDialogNode::Transfer);
    }
    return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}
