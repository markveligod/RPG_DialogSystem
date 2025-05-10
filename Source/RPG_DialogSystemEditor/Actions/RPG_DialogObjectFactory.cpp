// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogObjectFactory.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogSystemEditor/RPG_DialogSystemEditor.h"

#define LOCTEXT_NAMESPACE "DialogObjectFactory"

URPG_DialogObjectFactory::URPG_DialogObjectFactory()
{
    bCreateNew = true;
    bEditAfterNew = true;

    SupportedClass = URPG_DialogObjectBase::StaticClass();
}

UObject* URPG_DialogObjectFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    URPG_DialogObjectBase* NewObjectAsset = NewObject<URPG_DialogObjectBase>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

uint32 URPG_DialogObjectFactory::GetMenuCategories() const
{
    return FRPG_DialogSystemEditorModule::AssetDialogEditorCategory;
}

FText URPG_DialogObjectFactory::GetDisplayName() const
{
    return LOCTEXT("DialogObjectText", "Dialog");
}

FString URPG_DialogObjectFactory::GetDefaultNewAssetName() const
{
    return FString(TEXT("NewDialog"));
}

#undef LOCTEXT_NAMESPACE