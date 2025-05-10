

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "RPG_DialogGraphNode_Base.generated.h"

class URPG_DialogPlayer;
class URPG_DialogNodeWork;
class URPG_DialogNodeBase;
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

    virtual void InitNode(const URPG_DialogNodeBase* InObject);
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
    /** @public **/
    int32 GetTargetIndexNode() const { return TargetIndexNode; }

private:
    /** @private **/
    int32 TargetIndexNode{INDEX_NONE};

    /** @private **/
    TSharedPtr<SDialogGraphNode> DialogSGraphNode;

#pragma endregion

#pragma region ActionNode

public:
    /** @public **/
    URPG_DialogNodeBase* GetOwnerNode() const;

    /** @public **/
    UEdGraphPin* GetInputPin() const;

    /** @public **/
    UEdGraphPin* GetOutputPin() const;

    /** @public **/
    void AutoConnectionPins(bool bMarkDirty = true) const;

protected:
    /** @protected **/
    URPG_DialogGraphNode_Base* FindGraphNode(int32 TargetIndex) const;

    /** @protected **/
    URPG_DialogObjectBase* GetDialogObject() const;

    /** @protected **/
    UEdGraphPin* GetOutputPinByIndexWork(int32 IndexWork) const;

    /** @protected **/
    URPG_DialogPlayer* GetDialogPlayerByIndexPin(int32 IndexPin) const;

    /** @protected **/
    virtual void DialogGraphNode_EditChangeProperty(UObject* Object, struct FPropertyChangedEvent& PropertyChangedEvent);

    /** @protected **/
    void RebuildOutputPins();

    /** @protected **/
    void RemoveOutputPins();

    /** @protected **/
    TArray<UEdGraphPin*> GetOutputPins();

    /** @protected Break all output links on this node */
    void BreakAllOutputNodeLinks(bool bAlwaysMarkDirty = true);

private:
    /** @public **/
    FText GetNodeTitle(URPG_DialogNodeBase* Node) const;

    /** @private **/
    void MakeWorkOutputPins(URPG_DialogNodeWork* NodeWork);

#pragma endregion
};
