#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Dom/JsonObject.h"
#include "AssetRegistry/AssetData.h"
#include "DragAndDrop/ExternalContentDragDropOp.h"
#include "Input/DragAndDrop.h"
#include "PropertyCustomizationHelpers.h"  // brings in SObjectPropertyEntryBox

// Types of nodes in the JSON tree
enum class ENarrativeTreeItemType
{
    Root,
    Category,
    Object
};

// A single node in the JSON tree, now with its own BoundAsset
struct FNarrativeTreeItem : public TSharedFromThis<FNarrativeTreeItem>
{
    ENarrativeTreeItemType                Type;
    FString                                DisplayName;
    FString                                CategoryKey;
    TSharedPtr<FJsonObject>                DataObject;  // valid if Type==Object

    /** Per‐node bound UE asset */
    FAssetData                              BoundAsset;

    TArray<TSharedPtr<FNarrativeTreeItem>> Children;
};

/**
 * SNarrativeJsonBrowser
 */
class SNarrativeJsonBrowser : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SNarrativeJsonBrowser) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    void LoadJsonFromString(const FString& JsonString);

protected:
    virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
    virtual FReply OnDrop    (const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

private:
    bool                                   bHasJson = false;
    private:
    // The path of the JSON we currently have displayed
    FString CurrentJsonFilePath;
    TSharedPtr<FJsonObject>                RootJson;
    TArray<TSharedPtr<FNarrativeTreeItem>> RootItems;
    TSharedPtr< STreeView<TSharedPtr<FNarrativeTreeItem>> > TreeView;
    TSharedPtr<FNarrativeTreeItem>         SelectedItem;

    void BuildTree();
    TSharedRef<ITableRow> OnGenerateRow(
        TSharedPtr<FNarrativeTreeItem> Item,
        const TSharedRef<STableViewBase>& OwnerTable);
    void OnGetChildren(
        TSharedPtr<FNarrativeTreeItem> Item,
        TArray<TSharedPtr<FNarrativeTreeItem>>& OutChildren);
    void OnSelectionChanged(
        TSharedPtr<FNarrativeTreeItem> NewSelection,
        ESelectInfo::Type SelectInfo);

    
    FString GetBindingsFilePath() const
    {
        return FPaths::ProjectSavedDir() / TEXT("NarrativeAssetBindings.json");
    }
    FReply OnLoadLastJsonClicked();

    /** Load previously‐saved asset bindings from disk */
    void LoadBindings();

    /** Write out all current bindings to disk */
    void SaveBindings();

    /** Browse button action using the selected node’s BoundAsset */
    FReply OnBrowseToAssetClicked();

    // Detail getters
    FText GetSelectedName()    const;
    FText GetSelectedSummary() const;
    FReply OnBackToDropScreenClicked();
};
