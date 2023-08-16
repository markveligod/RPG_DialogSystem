#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class FRPG_DialogSystemEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

protected:
    /** @protected Creating a new category for the quest system **/
    void RegisterAssetCategoryAndActions() const;

public:
    static EAssetTypeCategories::Type AssetDialogEditorCategory;
};
