// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPG_DialogObjectFactory.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DIALOGSYSTEMEDITOR_API URPG_DialogObjectFactory : public UFactory
{
    GENERATED_BODY()

    URPG_DialogObjectFactory();
    
    // UFactory interface
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
    virtual uint32 GetMenuCategories() const override;
    virtual FText GetDisplayName() const override;
    virtual FString GetDefaultNewAssetName() const override;
    // End of UFactory interface
};
