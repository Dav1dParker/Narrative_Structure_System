// SNarrativeJsonBrowser.cpp

#include "SNarrativeJsonBrowser.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Input/SButton.h"
#include "Fonts/SlateFontInfo.h"
#include "PropertyCustomizationHelpers.h"
#include "ContentBrowserModule.h"
#include "EditorStyleSet.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DragAndDrop/ExternalContentDragDropOp.h"
#include "Misc/PackageName.h"

// Forward-declared Explorer drag op
class FExternalDragOperation;

//////////////////////////////////////////////////////////////////////////
// Helpers: persistence paths

static FString GetBindingsFilePath()
{
    return FPaths::ProjectSavedDir() / TEXT("NarrativeAssetBindings.json");
}

//////////////////////////////////////////////////////////////////////////
// SNarrativeJsonBrowser

void SNarrativeJsonBrowser::Construct(const FArguments& InArgs)
{
    const int32 BaseFontSize = 18;
    const int32 LargeFontSize = BaseFontSize + 4;
    const int32 TitleFontSize = BaseFontSize + 4;
    const int32 BackButtonFontSize = BaseFontSize - 4;

    // 1) Load persisted LastFile
    LoadBindings();

    // 2) Auto-reload if valid
    if (!CurrentJsonFilePath.IsEmpty() && FPaths::FileExists(CurrentJsonFilePath))
    {
        FString JsonString;
        if (FFileHelper::LoadFileToString(JsonString, *CurrentJsonFilePath))
        {
            LoadJsonFromString(JsonString);
            bHasJson = true;
            LoadBindings(); // reapply per-node asset bindings
        }
    }

    // 3) Build Slate UI
    ChildSlot
    [
        SNew(SBorder)
        .BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
        .Padding(8)
        [
            SNew(SWidgetSwitcher)
            .WidgetIndex_Lambda([this]() { return bHasJson ? 1 : 0; })

            // Slot 0: Drop prompt + Load Last JSON
            + SWidgetSwitcher::Slot()
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .FillHeight(1)
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(TEXT("Перетащите JSON файл сюда")))
                    .Justification(ETextJustify::Center)
                    .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), LargeFontSize))
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .VAlign(VAlign_Center)
                .HAlign(HAlign_Center)
                .Padding(0, 20)
                [
                    SNew(SButton)
                    .OnClicked(this, &SNarrativeJsonBrowser::OnLoadLastJsonClicked)
                    [
                        SNew(STextBlock)
                        .Text(FText::FromString(TEXT("Загрузить последний JSON")))
                        .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), BaseFontSize + 6))
                    ]
                ]
            ]

            // Slot 1: Tree + Details
            + SWidgetSwitcher::Slot()
            [
                SNew(SSplitter)

                // Left: TreeView
                + SSplitter::Slot().Value(0.3f)
                [
                    SNew(SBorder).Padding(4)
                    [
                        SAssignNew(TreeView, STreeView<TSharedPtr<FNarrativeTreeItem>>)
                        .TreeItemsSource(&RootItems)
                        .OnGenerateRow(this, &SNarrativeJsonBrowser::OnGenerateRow)
                        .OnGetChildren(this, &SNarrativeJsonBrowser::OnGetChildren)
                        .OnSelectionChanged(this, &SNarrativeJsonBrowser::OnSelectionChanged)
                    ]
                ]

                // Right: Details + asset picker
                + SSplitter::Slot().Value(0.7f)
                [
                    SNew(SScrollBox)
                    + SScrollBox::Slot().Padding(4)
                    [
                        SNew(SVerticalBox)

                        // Back button
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SButton)
                            .OnClicked(this, &SNarrativeJsonBrowser::OnBackToDropScreenClicked)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString(TEXT("Вернуться к импорту JSON")))
                                .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), BackButtonFontSize))
                            ]
                        ]

                        // Имя
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Имя:")))
                            .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), TitleFontSize))
                        ]
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(STextBlock)
                            .Text(this, &SNarrativeJsonBrowser::GetSelectedName)
                            .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), BaseFontSize))
                        ]

                        // Описание
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Описание:")))
                            .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), TitleFontSize))
                        ]
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(STextBlock)
                            .AutoWrapText(true)
                            .Text(this, &SNarrativeJsonBrowser::GetSelectedSummary)
                            .Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), BaseFontSize))
                        ]

                        // Asset picker
                        + SVerticalBox::Slot().AutoHeight().Padding(2)
                        [
                            SNew(SObjectPropertyEntryBox)
                            .AllowedClass(UObject::StaticClass())
                            .ObjectPath_Lambda([this]() {
                                return SelectedItem.IsValid()
                                    ? SelectedItem->BoundAsset.ObjectPath.ToString()
                                    : FString();
                            })
                            .OnObjectChanged_Lambda([this](const FAssetData& NewAsset) {
                                if (SelectedItem.IsValid())
                                {
                                    SelectedItem->BoundAsset = NewAsset;
                                    SaveBindings();
                                }
                            })
                            .AllowClear(false)
                            .Visibility_Lambda([this]() {
                                return (SelectedItem.IsValid() && SelectedItem->Type == ENarrativeTreeItemType::Object)
                                    ? EVisibility::Visible
                                    : EVisibility::Collapsed;
                            })
                        ]
                    ]
                ]
            ]
        ]
    ];
}

//////////////////////////////////////////////////////////////////////////
// JSON parsing + tree

void SNarrativeJsonBrowser::LoadJsonFromString(const FString& JsonString)
{
    RootJson.Reset();
    SelectedItem.Reset();
    RootItems.Empty();

    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    FJsonSerializer::Deserialize(Reader, RootJson);
    BuildTree();
}

void SNarrativeJsonBrowser::BuildTree()
{
    if (!RootJson.IsValid() || !RootJson->HasField(TEXT("project")))
    {
        return;
    }

    RootItems.Empty();

    auto ProjObj   = RootJson->GetObjectField(TEXT("project"));
    auto RootNode  = MakeShared<FNarrativeTreeItem>();
    RootNode->Type = ENarrativeTreeItemType::Root;
    RootNode->DisplayName = ProjObj->GetStringField(TEXT("name"));
    RootItems.Add(RootNode);

    static const TArray<FString> Categories = { TEXT("characters"), TEXT("locations"), TEXT("events") };
    for (const auto& Key : Categories)
    {
        if (!RootJson->HasField(Key)) continue;

        auto Arr     = RootJson->GetArrayField(Key);
        auto CatNode = MakeShared<FNarrativeTreeItem>();
        CatNode->Type        = ENarrativeTreeItemType::Category;
        CatNode->DisplayName = Key.Left(1).ToUpper() + Key.Mid(1);
        CatNode->CategoryKey = Key;
        RootNode->Children.Add(CatNode);

        for (auto& Val : Arr)
        {
            auto Obj  = Val->AsObject();
            auto Item = MakeShared<FNarrativeTreeItem>();
            Item->Type        = ENarrativeTreeItemType::Object;
            Item->DisplayName = Obj->GetStringField(TEXT("name"));
            Item->CategoryKey = Key;
            Item->DataObject  = Obj;
            CatNode->Children.Add(Item);
        }
    }

    if (TreeView.IsValid())
    {
        TreeView->RequestTreeRefresh();
    }
}

//////////////////////////////////////////////////////////////////////////
// Drag & Drop overrides

FReply SNarrativeJsonBrowser::OnDragOver(const FGeometry&, const FDragDropEvent& Event)
{
    if ( Event.GetOperationAs<FExternalDragOperation>() ||
         Event.GetOperationAs<FExternalContentDragDropOp>() )
    {
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply SNarrativeJsonBrowser::OnDrop(const FGeometry&, const FDragDropEvent& Event)
{
    // Explorer drop
    if (auto Op = Event.GetOperationAs<FExternalDragOperation>())
    {
        if (Op->HasFiles())
        {
            const auto& Files = Op->GetFiles();
            if (Files.Num() &&
                FPaths::GetExtension(Files[0]).Equals(TEXT("json"), ESearchCase::IgnoreCase))
            {
                CurrentJsonFilePath = Files[0];
                FString JsonString;
                if (FFileHelper::LoadFileToString(JsonString, *CurrentJsonFilePath))
                {
                    LoadJsonFromString(JsonString);
                    bHasJson = true;
                    if (TreeView.IsValid())
                    {
                        TreeView->RequestTreeRefresh();
                    }
                    SaveBindings();
                }
            }
        }
        return FReply::Handled();
    }

    // ContentBrowser drop (optional)
    if (auto CBOp = Event.GetOperationAs<FExternalContentDragDropOp>())
    {
        const auto& AssetPaths = CBOp->GetAssetPaths();
        if (AssetPaths.Num())
        {
            FString DiskPath = FPackageName::LongPackageNameToFilename(AssetPaths[0], TEXT(".json"));
            if (FPaths::FileExists(DiskPath))
            {
                CurrentJsonFilePath = DiskPath;
                FString JsonString;
                if (FFileHelper::LoadFileToString(JsonString, *DiskPath))
                {
                    LoadJsonFromString(JsonString);
                    bHasJson = true;
                    if (TreeView.IsValid())
                    {
                        TreeView->RequestTreeRefresh();
                    }
                    SaveBindings();
                }
            }
        }
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

//////////////////////////////////////////////////////////////////////////
// Persistence

void SNarrativeJsonBrowser::SaveBindings()
{
    const FString FullPath = GetBindingsFilePath();

    TSharedPtr<FJsonObject> RootObj;
    {
        FString In;
        if (FFileHelper::LoadFileToString(In, *FullPath))
        {
            auto R = TJsonReaderFactory<>::Create(In);
            FJsonSerializer::Deserialize(R, RootObj);
        }
    }
    if (!RootObj.IsValid())
    {
        RootObj = MakeShared<FJsonObject>();
    }

    // Serialize node bindings
    TArray<TSharedPtr<FJsonValue>> Arr;
    for (auto& RootItem : RootItems)
    {
        std::function<void(TSharedPtr<FNarrativeTreeItem>)> Recurse =
        [&](TSharedPtr<FNarrativeTreeItem> Item)
        {
            if (Item->Type==ENarrativeTreeItemType::Object &&
                Item->BoundAsset.IsValid() &&
                Item->DataObject.IsValid())
            {
                auto Entry = MakeShared<FJsonObject>();
                Entry->SetNumberField(TEXT("id"),        Item->DataObject->GetIntegerField(TEXT("id")));
                Entry->SetStringField(TEXT("assetPath"), Item->BoundAsset.ObjectPath.ToString());
                Arr.Add(MakeShared<FJsonValueObject>(Entry));
            }
            for (auto& C: Item->Children) Recurse(C);
        };
        Recurse(RootItem);
    }
    RootObj->SetArrayField(CurrentJsonFilePath, Arr);

    // Record last file
    RootObj->SetStringField(TEXT("LastFile"), CurrentJsonFilePath);

    // Save out
    FString Out;
    auto Writer = TJsonWriterFactory<>::Create(&Out);
    FJsonSerializer::Serialize(RootObj.ToSharedRef(), Writer);
    FFileHelper::SaveStringToFile(Out, *FullPath);
}

void SNarrativeJsonBrowser::LoadBindings()
{
    const FString FullPath = GetBindingsFilePath();
    FString JsonText;
    if (!FFileHelper::LoadFileToString(JsonText, *FullPath))
    {
        return;
    }

    TSharedPtr<FJsonObject> RootObj;
    {
        auto R = TJsonReaderFactory<>::Create(JsonText);
        if (!FJsonSerializer::Deserialize(R, RootObj) || !RootObj.IsValid())
        {
            return;
        }
    }

    // Restore LastFile
    if (RootObj->HasField(TEXT("LastFile")))
    {
        CurrentJsonFilePath = RootObj->GetStringField(TEXT("LastFile"));
    }

    // Reapply bindings only if tree exists
    if (RootItems.Num() && RootObj->HasField(CurrentJsonFilePath))
    {
        const auto& Arr = RootObj->GetArrayField(CurrentJsonFilePath);
        TMap<int32, FString> Map;
        for (auto& Val : Arr)
        {
            if (auto O = Val->AsObject())
            {
                Map.Add(
                    O->GetIntegerField(TEXT("id")),
                    O->GetStringField(TEXT("assetPath"))
                );
            }
        }

        for (auto& RootItem : RootItems)
        {
            std::function<void(TSharedPtr<FNarrativeTreeItem>)> Recurse =
            [&](TSharedPtr<FNarrativeTreeItem> Item)
            {
                if (Item->Type==ENarrativeTreeItemType::Object && Item->DataObject.IsValid())
                {
                    int32 Id = Item->DataObject->GetIntegerField(TEXT("id"));
                    if (auto* Path = Map.Find(Id))
                    {
                        FAssetData Data =
                            FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry")
                            .Get().GetAssetByObjectPath(FName(**Path));
                        Item->BoundAsset = Data;
                    }
                }
                for (auto& C: Item->Children) Recurse(C);
            };
            Recurse(RootItem);
        }
    }

    if (TreeView.IsValid())
    {
        TreeView->RequestTreeRefresh();
    }
}

//////////////////////////////////////////////////////////////////////////
// Tree callbacks

TSharedRef<ITableRow> SNarrativeJsonBrowser::OnGenerateRow(
    TSharedPtr<FNarrativeTreeItem> Item,
    const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(STableRow<TSharedPtr<FNarrativeTreeItem>>, OwnerTable)
    [
        SNew(STextBlock).Text(FText::FromString(Item->DisplayName))
    ];
}

void SNarrativeJsonBrowser::OnGetChildren(
    TSharedPtr<FNarrativeTreeItem> Item,
    TArray<TSharedPtr<FNarrativeTreeItem>>& OutChildren)
{
    OutChildren = Item->Children;
}

void SNarrativeJsonBrowser::OnSelectionChanged(
    TSharedPtr<FNarrativeTreeItem> NewSelection,
    ESelectInfo::Type)
{
    SelectedItem = NewSelection;
}

//////////////////////////////////////////////////////////////////////////
// Button handlers

FReply SNarrativeJsonBrowser::OnLoadLastJsonClicked()
{
    if (!CurrentJsonFilePath.IsEmpty() && FPaths::FileExists(CurrentJsonFilePath))
    {
        FString JsonString;
        if (FFileHelper::LoadFileToString(JsonString, *CurrentJsonFilePath))
        {
            LoadJsonFromString(JsonString);
            bHasJson = true;
            LoadBindings();
            if (TreeView.IsValid())
            {
                TreeView->RequestTreeRefresh();
            }
        }
    }
    return FReply::Handled();
}

FReply SNarrativeJsonBrowser::OnBackToDropScreenClicked()
{
    bHasJson = false;
    SelectedItem.Reset();
    RootItems.Empty();
    if (TreeView.IsValid())
    {
        TreeView->RequestTreeRefresh();
    }
    return FReply::Handled();
}

FReply SNarrativeJsonBrowser::OnBrowseToAssetClicked()
{
    if (SelectedItem.IsValid() && SelectedItem->BoundAsset.IsValid())
    {
        FContentBrowserModule& CBM = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
        CBM.Get().SyncBrowserToAssets({ SelectedItem->BoundAsset });
    }
    return FReply::Handled();
}

FText SNarrativeJsonBrowser::GetSelectedName() const
{
    if (SelectedItem.IsValid() && SelectedItem->Type == ENarrativeTreeItemType::Object)
    {
        return FText::FromString(SelectedItem->DataObject->GetStringField(TEXT("name")));
    }
    return FText::FromString(TEXT("--"));
}

FText SNarrativeJsonBrowser::GetSelectedSummary() const
{
    if (SelectedItem.IsValid() && SelectedItem->Type == ENarrativeTreeItemType::Object)
    {
        return FText::FromString(SelectedItem->DataObject->GetStringField(TEXT("summary")));
    }
    return FText::FromString(TEXT("--"));
}
