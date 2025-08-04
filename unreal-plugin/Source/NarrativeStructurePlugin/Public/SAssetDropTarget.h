#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DragAndDrop/ExternalContentDragDropOp.h"

// Forward‐declared Explorer drag op
class FExternalDragOperation;

struct FAssetDropArgs
{
    DECLARE_DELEGATE_OneParam(FOnAssetDropped, const FAssetData& /*Asset*/);

    SLATE_BEGIN_ARGS(FAssetDropArgs) {}
        SLATE_EVENT(FOnAssetDropped, OnAssetDropped)
    SLATE_END_ARGS()

    FOnAssetDropped OnAssetDropped;
};

class SAssetDropTarget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SAssetDropTarget) {}
        SLATE_EVENT(FAssetDropArgs::FOnAssetDropped, OnAssetDropped)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
    {
        DropDelegate = InArgs._OnAssetDropped;

        ChildSlot
        [
            SNew(SBorder)
            .BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
            .Padding(6)
            [
                SNew(STextBlock)
                .Justification(ETextJustify::Center)
                .Text_Lambda([this]()
                {
                    return BoundAsset.IsValid()
                        ? FText::FromName(BoundAsset.AssetName)
                        : FText::FromString(TEXT("Drop Blueprint here"));
                })
            ]
        ];
    }

protected:
    virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
    {
        if (DragDropEvent.GetOperationAs<FExternalContentDragDropOp>() ||
            DragDropEvent.GetOperationAs<FExternalDragOperation>())
        {
            return FReply::Handled();
        }
        return FReply::Unhandled();
    }

    virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
    {
        // Content Browser drop
        if (auto ContentOp = DragDropEvent.GetOperationAs<FExternalContentDragDropOp>())
        {
            const auto& Paths = ContentOp->GetAssetPaths();
            if (Paths.Num())
            {
                FAssetData Data = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry")
                    .Get()
                    .GetAssetByObjectPath(FName(*Paths[0]));

                if (Data.IsValid())
                {
                    BoundAsset = Data;
                    DropDelegate.ExecuteIfBound(Data);
                }
            }
            return FReply::Handled();
        }

        // Explorer drop (.uasset)
        if (auto ExplorerOp = DragDropEvent.GetOperationAs<FExternalDragOperation>())
        {
            if (ExplorerOp->HasFiles())
            {
                const auto& Files = ExplorerOp->GetFiles();
                if (Files.Num() && FPaths::GetExtension(Files[0]).Equals(TEXT("uasset"), ESearchCase::IgnoreCase))
                {
                    FString PackageName = FPackageName::FilenameToLongPackageName(Files[0]);
                    FAssetData Data = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry")
                        .Get()
                        .GetAssetByObjectPath(FName(*PackageName));

                    if (Data.IsValid())
                    {
                        BoundAsset = Data;
                        DropDelegate.ExecuteIfBound(Data);
                    }
                }
            }
            return FReply::Handled();
        }

        return FReply::Unhandled();
    }

private:
    FAssetData BoundAsset;
    FAssetDropArgs::FOnAssetDropped DropDelegate;
};
