#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystemDataTypes.generated.h"

UENUM(BlueprintType)
enum class ERPG_TypeStateDialog: uint8
{
    None        UMETA(Hidden),
    Entry,
    NPCNode,
    PlayerNode,
};

USTRUCT(BlueprintType)
struct FRPG_DialogNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 IndexNode{INDEX_NONE};

    UPROPERTY(EditAnywhere)
    FVector2D NodePosition{FVector2D::UnitVector};

    UPROPERTY(EditAnywhere)
    ERPG_TypeStateDialog TypeStateDialog{ERPG_TypeStateDialog::None};

    UPROPERTY(EditAnywhere)
    URPG_DialogConditionObject* DialogConditionObject{nullptr};

    UPROPERTY(EditAnywhere)
    TArray<int32> OutNodes;

    bool operator==(const FRPG_DialogNode& Node) const
    {
        return Node.IndexNode == this->IndexNode;
    }
};


