#pragma once

#include "AssetTypeActions_Base.h"

class FRPG_DialogAssetTypeActions : public FAssetTypeActions_Base
{
public:
    FRPG_DialogAssetTypeActions();

    // IAssetTypeActions interface
    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
    virtual uint32 GetCategories() override;
    // End of IAssetTypeActions interface
};
