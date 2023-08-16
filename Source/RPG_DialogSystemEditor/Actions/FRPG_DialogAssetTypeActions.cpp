#include "FRPG_DialogAssetTypeActions.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystemEditor/RPG_DialogSystemEditor.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FRPG_DialogAssetTypeActions::FRPG_DialogAssetTypeActions() {}

FText FRPG_DialogAssetTypeActions::GetName() const
{
    return LOCTEXT("FDialogAssetTypeActionsName", "Dialog");
}

FColor FRPG_DialogAssetTypeActions::GetTypeColor() const
{
    return FColor::Green;
}

UClass* FRPG_DialogAssetTypeActions::GetSupportedClass() const
{
    return URPG_DialogObjectBase::StaticClass();
}

void FRPG_DialogAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

    for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        if (URPG_DialogObjectBase* Quest = Cast<URPG_DialogObjectBase>(*ObjIt))
        {
        }
    }
}

uint32 FRPG_DialogAssetTypeActions::GetCategories()
{
    return FRPG_DialogSystemEditorModule::AssetDialogEditorCategory;
}

#undef LOCTEXT_NAMESPACE
