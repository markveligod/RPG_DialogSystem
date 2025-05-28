#include "RPG_DialogAssetEditor.h"
#include "FileHelpers.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/RPG_DialogEdGraphSchema.h"
#include "Graph/RPG_DialogGraph.h"
#include "Graph/Nodes/RPG_DialogGraphNode_Base.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"

#define LOCTEXT_NAMESPACE "DialogAssetEditor"

const FName DialogEditorAppName = FName(TEXT("DialogEditorApp"));

struct FDialogEditorTabs
{
    // Tab identifiers
    static const FName DialogDetailsID;
    static const FName DialogViewportID;

    static const FText DialogLocalWorkspace;

    static const FText DialogViewportTabDisplayName;
    static const FText DialogDetailsTabDisplayName;

    // Style name
    static const FName DialogViewportTabSlateIcon;
    static const FName DialogDetailsTabSlateIcon;

    static const FText DialogViewportGraphTitle;
    static const FText DialogDetailsGraphTitle;
};

const FName FDialogEditorTabs::DialogDetailsID(TEXT("DialogDetails"));
const FName FDialogEditorTabs::DialogViewportID(TEXT("DialogViewport"));
const FText FDialogEditorTabs::DialogLocalWorkspace(FText::FromString(TEXT("LocalWorkspaceMenu_DialogueEditor")));
const FText FDialogEditorTabs::DialogViewportTabDisplayName(FText::FromString(TEXT("DialogViewportTab")));
const FText FDialogEditorTabs::DialogDetailsTabDisplayName(FText::FromString(TEXT("DialogDetailsTab")));
const FName FDialogEditorTabs::DialogViewportTabSlateIcon(TEXT("LevelEditor.Tabs.Viewports"));
const FName FDialogEditorTabs::DialogDetailsTabSlateIcon(TEXT("LevelEditor.Tabs.Details"));
const FText FDialogEditorTabs::DialogViewportGraphTitle(FText::FromString(TEXT("DialogGraph")));
const FText FDialogEditorTabs::DialogDetailsGraphTitle(FText::FromString(TEXT("DialogDetails")));

FRPG_DialogAssetEditor::FRPG_DialogAssetEditor() {}

FRPG_DialogAssetEditor::~FRPG_DialogAssetEditor() {}

void FRPG_DialogAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(FDialogEditorTabs::DialogLocalWorkspace);
    auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    InTabManager->RegisterTabSpawner(FDialogEditorTabs::DialogViewportID, FOnSpawnTab::CreateSP(this, &FRPG_DialogAssetEditor::SpawnTab_Viewport))
        .SetDisplayName(FDialogEditorTabs::DialogViewportTabDisplayName)
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), FDialogEditorTabs::DialogViewportTabSlateIcon));

    InTabManager->RegisterTabSpawner(FDialogEditorTabs::DialogDetailsID, FOnSpawnTab::CreateSP(this, &FRPG_DialogAssetEditor::SpawnTab_Details))
        .SetDisplayName(FDialogEditorTabs::DialogDetailsTabDisplayName)
        .SetGroup(WorkspaceMenuCategoryRef)
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), FDialogEditorTabs::DialogDetailsTabSlateIcon));
}

void FRPG_DialogAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

    InTabManager->UnregisterTabSpawner(FDialogEditorTabs::DialogViewportID);
    InTabManager->UnregisterTabSpawner(FDialogEditorTabs::DialogDetailsID);
}

FName FRPG_DialogAssetEditor::GetToolkitFName() const
{
    return FName("DialogToolkit");
}

FText FRPG_DialogAssetEditor::GetBaseToolkitName() const
{
    return FText::FromString(TEXT("BaseDialogToolkit"));
}

FText FRPG_DialogAssetEditor::GetToolkitName() const
{
    return FAssetEditorToolkit::GetToolkitName();
}

FText FRPG_DialogAssetEditor::GetToolkitToolTipText() const
{
    return FAssetEditorToolkit::GetToolkitToolTipText();
}

FLinearColor FRPG_DialogAssetEditor::GetWorldCentricTabColorScale() const
{
    return FLinearColor::White;
}

FString FRPG_DialogAssetEditor::GetWorldCentricTabPrefix() const
{
    return TEXT("DialogAssetEditor");
}

bool FRPG_DialogAssetEditor::IsAssetEditor() const
{
    return FAssetEditorToolkit::IsAssetEditor();
}

bool FRPG_DialogAssetEditor::ProcessCommandBindings(const FKeyEvent& InKeyEvent) const
{
    return FAssetEditorToolkit::ProcessCommandBindings(InKeyEvent);
}

void FRPG_DialogAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
    Collector.AddReferencedObject(DialogBeingEdited);
}

FString FRPG_DialogAssetEditor::GetReferencerName() const
{
    return DialogBeingEdited ? DialogBeingEdited->GetName() : TEXT("DialogAssetEditor_ReferencerName");
}

void FRPG_DialogAssetEditor::BindGraphCommands()
{
    const FGenericCommands& GenericCommands = FGenericCommands::Get();

    ToolkitCommands->MapAction(
        GenericCommands.Delete, FExecuteAction::CreateSP(this, &FRPG_DialogAssetEditor::DeleteSelectedNodes), FCanExecuteAction::CreateSP(this, &FRPG_DialogAssetEditor::CanDeleteNodes));
}

void FRPG_DialogAssetEditor::InitDialogEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, URPG_DialogObjectBase* InitDialogObject)
{
    DialogBeingEdited = InitDialogObject;
    TSharedPtr<FRPG_DialogAssetEditor> DialogEditorPtr = SharedThis(this);

    BindGraphCommands();
    // Default layout
    const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout =
        FTabManager::NewLayout("Standalone_DialogEditor_Layout_v3")
            ->AddArea(FTabManager::NewPrimaryArea()
                    ->SetOrientation(Orient_Vertical)
                    ->Split(FTabManager::NewSplitter()
                            ->SetOrientation(Orient_Horizontal)
                            ->SetSizeCoefficient(0.9f)
                            ->Split(FTabManager::NewStack()->SetSizeCoefficient(0.40f)->AddTab(FDialogEditorTabs::DialogDetailsID, ETabState::OpenedTab))
                            ->Split(FTabManager::NewStack()->SetSizeCoefficient(0.50f)->AddTab(FDialogEditorTabs::DialogViewportID, ETabState::OpenedTab))));

    // Initialize the asset editor
    InitAssetEditor(Mode, InitToolkitHost, DialogEditorAppName, StandaloneDefaultLayout, true, true, InitDialogObject);
    RegenerateMenusAndToolbars();
}

TSharedRef<SDockTab> FRPG_DialogAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
    TSharedPtr<FRPG_DialogAssetEditor> QuestEditorPtr = SharedThis(this);

    TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab).Label(FDialogEditorTabs::DialogViewportGraphTitle);

    FocusedGraphEditor = CreateGraphWidget();
    if (FocusedGraphEditor.IsValid())
    {
        DialogEdGraph->GetSchema()->CreateDefaultNodesForGraph(*DialogEdGraph);
        SpawnedTab->SetContent(FocusedGraphEditor.ToSharedRef());
    }

    return SpawnedTab;
}

TSharedRef<SDockTab> FRPG_DialogAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
    TSharedPtr<FRPG_DialogAssetEditor> QuestEditorPtr = SharedThis(this);

    FDetailsViewArgs DetailsArgs;
    DetailsArgs.bHideSelectionTip = true;
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    DialogProperties = PropertyModule.CreateDetailView(DetailsArgs);

    // Spawn the tab
    return SNew(SDockTab).Label(FDialogEditorTabs::DialogDetailsGraphTitle)[DialogProperties.ToSharedRef()];
}

TSharedRef<SGraphEditor> FRPG_DialogAssetEditor::CreateGraphWidget()
{
    SGraphEditor::FGraphEditorEvents InEvents;
    InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FRPG_DialogAssetEditor::OnSelectedNodesChanged);
    InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FRPG_DialogAssetEditor::OnNodeTitleCommitted);
    DialogEdGraph = Cast<URPG_DialogGraph>(FBlueprintEditorUtils::CreateNewGraph(DialogBeingEdited, "Dialog Graph", URPG_DialogGraph::StaticClass(), URPG_DialogEdGraphSchema::StaticClass()));

    return SNew(SGraphEditor)
        .AdditionalCommands(ToolkitCommands)
        .IsEditable(true)
        .Appearance(GetGraphAppearanceInfo())
        .GraphToEdit(DialogEdGraph)
        .GraphEvents(InEvents)
        .AutoExpandActionMenu(true)
        .ShowGraphStateOverlay(true)
        .AssetEditorToolkit(SharedThis(this));
}

FGraphAppearanceInfo FRPG_DialogAssetEditor::GetGraphAppearanceInfo()
{
    FGraphAppearanceInfo NewInfo;
    NewInfo.CornerText = FText::FromString(FString(TEXT("DIALOGUE")));
    return NewInfo;
}

void FRPG_DialogAssetEditor::OnSelectedNodesChanged(const TSet<UObject*>& Nodes)
{
    TArray<UObject*> Objects;

    for (auto Node : Nodes)
    {
        URPG_DialogGraphNode_Base* DialogGraphNode = Cast<URPG_DialogGraphNode_Base>(Node);
        if (!DialogGraphNode) continue;

        URPG_DialogNodeBase* DialogNodeBase = DialogGraphNode->GetOwnerNode();
        if (!DialogNodeBase) continue;

        if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Work || DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Transfer)
        {
            Objects.Add(DialogNodeBase);
        }
    }

    DialogProperties->SetObjects(Objects);
}

void FRPG_DialogAssetEditor::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged) {}

void FRPG_DialogAssetEditor::DeleteSelectedNodes()
{
    if (!DialogBeingEdited) return;
    const FScopedTransaction Transaction(LOCTEXT("DeleteSelectedNode", "Delete Selected Node"));
    FocusedGraphEditor->GetCurrentGraph()->Modify();

    const FGraphPanelSelectionSet SelectedNodes = FocusedGraphEditor->GetSelectedNodes();

    for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
    {
        UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);
        if (Node && Node->CanUserDeleteNode())
        {
            URPG_DialogGraphNode_Base* DialogGraphNode = Cast<URPG_DialogGraphNode_Base>(Node);
            if (!DialogGraphNode) continue;

            DialogGraphNode->ResetNode();
            DialogBeingEdited->RemoveNode(DialogGraphNode->GetTargetIndexNode());
            FBlueprintEditorUtils::RemoveNode(nullptr, Node, true);
        }
    }

    if (SelectedNodes.Num() > 0 && DialogBeingEdited && DialogBeingEdited->GetClass())
    {
        DialogBeingEdited->Modify();
        DialogBeingEdited->GetClass()->Modify();
        TArray<UPackage*> PackagesToSave;
        PackagesToSave.Add(DialogBeingEdited->GetClass()->GetOutermost());
        FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
    }
}

bool FRPG_DialogAssetEditor::CanDeleteNodes() const
{
    const FGraphPanelSelectionSet SelectedNodes = FocusedGraphEditor->GetSelectedNodes();
    for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
    {
        const URPG_DialogGraphNode_Base* Node = Cast<URPG_DialogGraphNode_Base>(*NodeIt);
        if (!Node) continue;

        const URPG_DialogNodeBase* DialogNodeBase = Node->GetOwnerNode();
        if (!DialogNodeBase) continue;

        if (DialogNodeBase->GetTypeDialogNode() == ERPG_TypeDialogNode::Start)
        {
            return false;
        }
    }

    return SelectedNodes.Num() > 0;
}

#undef LOCTEXT_NAMESPACE
