#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "RPG_DialogGraphActions.generated.h"

USTRUCT()
struct RPG_DIALOGSYSTEMEDITOR_API FRPG_DialogAction_NewNPCNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FRPG_DialogAction_NewNPCNode() : FEdGraphSchemaAction(FText::FromString(TEXT("Action Graph")), FText::FromString(TEXT("Create NPC Node")), FText::FromString(TEXT("Generates a new NPC node")), 0)
    {
        
    }

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
    
    virtual FName GetTypeId() const override { return StaticGetTypeId(); }

    static const FName& StaticGetTypeId()
    {
        static FName Type("FDialogEditorGraphSchemaAction_NewNPCNode");
        return Type;
    }
};

USTRUCT()
struct RPG_DIALOGSYSTEMEDITOR_API FRPG_DialogAction_NewPlayerNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FRPG_DialogAction_NewPlayerNode() : FEdGraphSchemaAction(FText::FromString(TEXT("Action Graph")), FText::FromString(TEXT("Create Player Node")), FText::FromString(TEXT("Generates a new Player node")), 0)
    {
        
    }

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
    
    virtual FName GetTypeId() const override { return StaticGetTypeId(); }

    static const FName& StaticGetTypeId()
    {
        static FName Type("FDialogEditorGraphSchemaAction_NewPlayerNode");
        return Type;
    }
};

USTRUCT()
struct RPG_DIALOGSYSTEMEDITOR_API FRPG_DialogAction_NewTransferNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FRPG_DialogAction_NewTransferNode() : FEdGraphSchemaAction(FText::FromString(TEXT("Action Graph")), FText::FromString(TEXT("Create Transfer Node")), FText::FromString(TEXT("Generates a new Transfer node")), 0)
    {
        
    }

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
    
    virtual FName GetTypeId() const override { return StaticGetTypeId(); }

    static const FName& StaticGetTypeId()
    {
        static FName Type("FDialogEditorGraphSchemaAction_NewTransferNode");
        return Type;
    }
};
