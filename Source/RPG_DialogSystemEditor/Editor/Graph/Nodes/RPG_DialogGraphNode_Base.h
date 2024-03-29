﻿

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "RPG_DialogGraphNode_Base.generated.h"

class URPG_DialogSettingsObject;
class URPG_DialogObjectBase;
class SDialogGraphNode;
/**
 * 
 */
UCLASS()
class RPG_DIALOGSYSTEMEDITOR_API URPG_DialogGraphNode_Base : public UEdGraphNode
{
    GENERATED_BODY()

public:

    URPG_DialogGraphNode_Base();

    virtual bool CanUserDeleteNode() const override { return true; }
    virtual bool CanDuplicateNode() const override { return false; }
#pragma region EdGraphNodeInterface

public:
    // UEdGraphNode interface.
    virtual void AllocateDefaultPins() override;
    virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FText GetTooltipText() const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FLinearColor GetNodeCommentColor() const override;
    virtual FLinearColor GetNodeBodyTintColor() const override;
    virtual void NodeConnectionListChanged() override;
    virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
    // End of UEdGraphNode interface.

#pragma endregion

#pragma region DataNode

public:

    int32 TargetIndexTaskNode{INDEX_NONE};

    UEdGraphPin* OutputPin{nullptr};
    UEdGraphPin* InPin{nullptr};

private:
    
    TSharedPtr<SDialogGraphNode> DialogSGraphNode;
#pragma endregion

#pragma region ActionNode

public:

    URPG_DialogSettingsObject* GetDialogSettingsObject() const;

    void MakeLink(const URPG_DialogGraphNode_Base* To) const;

protected:

    URPG_DialogObjectBase* GetDialogObject() const;

    virtual void DialogGraphNode_EditChangeProperty(UObject* Object, struct FPropertyChangedEvent& PropertyChangedEvent);
    
#pragma endregion
    
};
