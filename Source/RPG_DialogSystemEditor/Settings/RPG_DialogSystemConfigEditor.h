// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogSystemConfigEditor.generated.h"

/**
 *
 */
UCLASS(Config = "Game", defaultconfig, meta = (DisplayName = "Dialog System Settings"))
class RPG_DIALOGSYSTEMEDITOR_API URPG_DialogSystemConfigEditor : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    URPG_DialogSystemConfigEditor();

    UPROPERTY(Config, EditAnywhere, Category = "Settings Editor")
    FLinearColor SchemeTypePinColor{FLinearColor::White};

    UPROPERTY(Config, EditAnywhere, Category = "Settings Editor")
    FLinearColor SchemeSecondaryPinColor{FLinearColor::White};

    UPROPERTY(Config, EditAnywhere, Category = "Settings Editor")
    FLinearColor IconEventColor{FLinearColor::Red};

    UPROPERTY(Config, EditAnywhere, Category = "Settings Editor")
    FLinearColor IconConditionColor{FColor::Cyan};

    UPROPERTY(Config, EditAnywhere, Category = "Settings Editor")
    TMap<ERPG_TypeDialogNode, FLinearColor> NodeTitleColors;

    UPROPERTY(Config, EditAnywhere, Category = "Settings Editor")
    TMap<ERPG_TypeDialogNode, FLinearColor> NodeBodyTintColors;
};
