#pragma once
#include "treedata_export.h"
#include <RawData.h>
#include <Set.h>
#include <QJsonObject>

using namespace mv;
using namespace mv::plugin;

// =============================================================================
// Data Type
// =============================================================================

const mv::DataType TreeType = mv::DataType(QString("Tree"));

// =============================================================================
// Raw Data
// =============================================================================

class TREEDATA_EXPORT TreeData : public mv::plugin::RawData
{
public:

    TreeData(PluginFactory* factory) : mv::plugin::RawData(factory, TreeType) { }
    ~TreeData(void) override;

    void init() override;

    /**
     * Create dataset for raw data
     * @param guid Globally unique dataset identifier (use only for deserialization)
     * @return Smart pointer to dataset
     */
    Dataset<mv::DatasetImpl> createDataSet(const QString& guid = "") const override;
    void setData(QJsonObject jsonString);
    QJsonObject& getData();
    QStringList& getSpeciesNames();

private:
    QJsonObject _data;
    QStringList _speciesNames;
};

class  TREEDATA_EXPORT Tree : public mv::DatasetImpl
{
public:
    Tree(QString dataName, bool mayUnderive = true, const QString& guid = "") :
        mv::DatasetImpl(dataName, mayUnderive, guid)
    {
        
    }

    ~Tree() override { }

    Dataset<mv::DatasetImpl> copy() const override
    {
        auto text = new Tree(getRawDataName());

        text->setText(this->text());
        text->indices = indices;

        return text;
    }

    /**
     * Create subset from the current selection and specify where the subset will be placed in the data hierarchy
     * @param guiName Name of the subset in the GUI
     * @param parentDataSet Smart pointer to parent dataset in the data hierarchy (default is below the set)
     * @param visible Whether the subset will be visible in the UI
     * @return Smart pointer to the created subset
     */
    Dataset<mv::DatasetImpl> createSubsetFromSelection(const QString& guiName, const Dataset<DatasetImpl>& parentDataSet = Dataset<mv::DatasetImpl>(), const bool& visible = true) const override
    {
        return mv::data().createSubsetFromSelection(getSelection(), toSmartPointer(), guiName, parentDataSet, visible);
    }

    /**
     * Get plugin icon
     * @param color Icon color for flat (font) icons
     * @return Icon
     */
    QIcon getIcon(const QColor& color = Qt::black) const override;

public: // Selection

    /**
     * Get selection
     * @return Selection indices
     */
    std::vector<std::uint32_t>& getSelectionIndices() override;

    /**
     * Select by indices
     * @param indices Selection indices
     */
    void setSelectionIndices(const std::vector<std::uint32_t>& indices) override;

    /** Determines whether items can be selected */
    bool canSelect() const override;

    /** Determines whether all items can be selected */
    bool canSelectAll() const override;

    /** Determines whether there are any items which can be deselected */
    bool canSelectNone() const override;

    /** Determines whether the item selection can be inverted (more than one) */
    bool canSelectInvert() const override;

    /** Select all items */
    void selectAll() override;

    /** De-select all items */
    void selectNone() override;

    /** Invert item selection */
    void selectInvert() override;
    void setData(QJsonObject jsonString);
    QJsonObject& getData();
    QStringList& getSpeciesNames();

    std::vector<unsigned int> indices;

};

// =============================================================================
// Factory
// =============================================================================

class TreeDataFactory : public RawDataFactory
{
    Q_INTERFACES(mv::plugin::RawDataFactory mv::plugin::PluginFactory)
        Q_OBJECT
        Q_PLUGIN_METADATA(IID   "nl.BioVault.TreeData"
            FILE  "TreeData.json")

public:
    TreeDataFactory(void) {}
    ~TreeDataFactory(void) override {}

    /**
     * Get plugin icon
     * @param color Icon color for flat (font) icons
     * @return Icon
     */
    QIcon getIcon(const QColor& color = Qt::black) const override;

    mv::plugin::RawData* produce() override;
};
