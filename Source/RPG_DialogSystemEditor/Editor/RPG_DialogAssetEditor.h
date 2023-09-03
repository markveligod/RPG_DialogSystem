/** Copyright Mark Veligod. Published in 2023. **/

#pragma once

#include "Toolkits/AssetEditorToolkit.h"
#include "EditorUndoClient.h"
#include "Framework/Commands/UICommandList.h"

class URPG_DialogGraph;
class URPG_DialogObjectBase;

class FRPG_DialogAssetEditor : public FAssetEditorToolkit, public FEditorUndoClient, FGCObject
{
public:
    
    FRPG_DialogAssetEditor();
    virtual ~FRPG_DialogAssetEditor();

    // IToolkit interface
    virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
    // End of IToolkit interface

    //~ Begin FEditorUndoClient Interface
    virtual void PostUndo(bool bSuccess) override;
    virtual void PostRedo(bool bSuccess) override;
    // End of FEditorUndoClient

    // FAssetEditorToolkit
    virtual FName GetToolkitFName() const override;     // Must implement in derived class!
    virtual FText GetBaseToolkitName() const override;  // Must implement in derived class!
    virtual FText GetToolkitName() const override;
    virtual FText GetToolkitToolTipText() const override;

    virtual FLinearColor GetWorldCentricTabColorScale() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual bool IsAssetEditor() const override;
    virtual bool ProcessCommandBindings(const FKeyEvent& InKeyEvent) const override;
    // End of FAssetEditorToolkit

    // FSerializableObject interface
    virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
    virtual FString GetReferencerName() const override;
    // End of FSerializableObject interface

    virtual void BindGraphCommands();

    void InitDialogEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, URPG_DialogObjectBase* InitDialogObject);
    URPG_DialogObjectBase* GetDialogBeingEdited() const { return DialogBeingEdited; }

protected:
    TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
    TSharedRef<SGraphEditor> CreateGraphWidget();
    FGraphAppearanceInfo GetGraphAppearanceInfo();
    void OnSelectedNodesChanged(const TSet<UObject*>& Nodes);
    void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);

    void CreateNPCNode();
    void CreatePlayerNode();
private:
    /** The object we're currently editing */
    URPG_DialogObjectBase* DialogBeingEdited{nullptr};
    /** Properties tab */
    TSharedPtr<class IDetailsView> DialogProperties;

    TSharedPtr<SGraphEditor> FocusedGraphEditor;
    TSharedPtr<class SEditorPaletteTasks> Palette;
    URPG_DialogGraph* DialogEdGraph{nullptr};
};
