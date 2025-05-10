#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "RPG_DialogGraphActions.generated.h"

USTRUCT()
struct RPG_DIALOGSYSTEMEDITOR_API FRPG_DialogAction_NewWorkNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FRPG_DialogAction_NewWorkNode()
        : FEdGraphSchemaAction(FText::FromString(TEXT("Action Graph")), FText::FromString(TEXT("Create Work Node")), FText::FromString(TEXT("Generates a new Work node")), 0)
    {
    }

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

    virtual FName GetTypeId() const override { return StaticGetTypeId(); }

    static const FName& StaticGetTypeId()
    {
        static FName Type("FDialogEditorGraphSchemaAction_NewWorkNode");
        return Type;
    }
};

USTRUCT()
struct RPG_DIALOGSYSTEMEDITOR_API FRPG_DialogAction_NewFinishNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FRPG_DialogAction_NewFinishNode()
        : FEdGraphSchemaAction(FText::FromString(TEXT("Action Graph")), FText::FromString(TEXT("Create Finish Node")), FText::FromString(TEXT("Generates a new Finish node")), 0)
    {
    }

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

    virtual FName GetTypeId() const override { return StaticGetTypeId(); }

    static const FName& StaticGetTypeId()
    {
        static FName Type("FDialogEditorGraphSchemaAction_NewFinishNode");
        return Type;
    }
};

USTRUCT()
struct RPG_DIALOGSYSTEMEDITOR_API FRPG_DialogAction_NewTransferNode : public FEdGraphSchemaAction
{
    GENERATED_BODY()

    FRPG_DialogAction_NewTransferNode()
        : FEdGraphSchemaAction(FText::FromString(TEXT("Action Graph")), FText::FromString(TEXT("Create Transfer Node")), FText::FromString(TEXT("Generates a new Transfer node")), 0)
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
