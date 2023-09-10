#include "RPG_DialogGraphActions.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogSystemEditor/Editor/Graph/RPG_DialogEdGraphSchema.h"

UEdGraphNode* FRPG_DialogAction_NewNPCNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    if (ParentGraph)
    {
        return CastChecked<URPG_DialogEdGraphSchema>(ParentGraph->GetSchema())->CreateStandardNodeForGraph(ParentGraph, Location, ERPG_TypeStateDialog::NPCNode);
    }
    return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}

UEdGraphNode* FRPG_DialogAction_NewPlayerNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    if (ParentGraph)
    {
        return CastChecked<URPG_DialogEdGraphSchema>(ParentGraph->GetSchema())->CreateStandardNodeForGraph(ParentGraph, Location, ERPG_TypeStateDialog::PlayerNode);
    }
    return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}

UEdGraphNode* FRPG_DialogAction_NewTransferNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
    if (ParentGraph)
    {
        return CastChecked<URPG_DialogEdGraphSchema>(ParentGraph->GetSchema())->CreateStandardNodeForGraph(ParentGraph, Location, ERPG_TypeStateDialog::Transfer);
    }
    return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}
