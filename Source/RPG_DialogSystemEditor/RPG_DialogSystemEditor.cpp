#include "RPG_DialogSystemEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Actions/FRPG_DialogAssetTypeActions.h"
#include "Style/FRPG_DialogSystemStyle.h"

#define LOCTEXT_NAMESPACE "FRPG_DialogSystemEditorModule"

EAssetTypeCategories::Type FRPG_DialogSystemEditorModule::AssetDialogEditorCategory = static_cast<EAssetTypeCategories::Type>(0);

void FRPG_DialogSystemEditorModule::StartupModule()
{
    FRPG_DialogSystemStyle::Initialize();
    RegisterAssetCategoryAndActions();
}

void FRPG_DialogSystemEditorModule::ShutdownModule()
{
    FRPG_DialogSystemStyle::Shutdown();
}

void FRPG_DialogSystemEditorModule::RegisterAssetCategoryAndActions() const
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetDialogEditorCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DialogSystemPlugin")), LOCTEXT("Dialog", "Dialog Plugin"));
    AssetTools.RegisterAssetTypeActions(MakeShareable(new FRPG_DialogAssetTypeActions()));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRPG_DialogSystemEditorModule, RPG_DialogSystemEditor)