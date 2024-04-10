#include "CrossSpeciesComparisonTreeData.h"
#include "InfoAction.h"
#include <Application.h>

#include <QtCore>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <algorithm>
#include <iostream>
#include <iostream>
#include <QJsonDocument>
#include <QJsonValue>

Q_PLUGIN_METADATA(IID "nl.BioVault.CrossSpeciesComparisonTreeData")

using namespace mv;



CrossSpeciesComparisonTreeData::~CrossSpeciesComparisonTreeData(void)
{

}

void CrossSpeciesComparisonTreeData::fromVariantMap(const QVariantMap& variantMap)
{
  /*
    variantMapMustContain(variantMap, "CrossSpeciesComparisonTreeInfo");
    variantMapMustContain(variantMap, "LeafInfo");

    const auto data = variantMap["Data"].toMap();
    const auto numberOfPoints = static_cast<size_t>(variantMap["NumberOfPoints"].toInt());
    const auto numberOfDimensions = variantMap["NumberOfDimensions"].toUInt();
    const auto numberOfElements = numberOfPoints * numberOfDimensions;
    const auto elementTypeIndex = static_cast<PointData::ElementTypeSpecifier>(data["TypeIndex"].toInt());
    const auto rawData = data["Raw"].toMap();


    setElementTypeSpecifier(elementTypeIndex);
    resizeVector(numberOfElements);
    populateDataBufferFromVariantMap(rawData, (char*)getDataVoidPtr());
    _numDimensions = numberOfDimensions;
*/
}

QVariantMap CrossSpeciesComparisonTreeData::toVariantMap() const
{
    QVariantMap rawData;
    /*
    const auto typeSpecifier = getElementTypeSpecifier();
    const auto typeSpecifierName = getElementTypeNames()[static_cast<std::int32_t>(typeSpecifier)];
    const auto typeIndex = static_cast<std::int32_t>(typeSpecifier);
    const auto numberOfElements = getNumberOfElements();

    QVariantMap rawData = rawDataToVariantMap((const char*)getDataConstVoidPtr(), getRawDataSize(), true);

    return {
        { "TypeIndex", QVariant::fromValue(typeIndex) },
        { "TypeName", QVariant(typeSpecifierName) },
        { "Raw", QVariant::fromValue(rawData) },
        { "NumberOfElements", QVariant::fromValue(numberOfElements) }
    };

    */
    return rawData;
}

void CrossSpeciesComparisonTreeData::init()
{


}


void extractNames(const QJsonObject& obj, QStringList& names) {
    if (obj.contains("name")) {
        names.append(obj["name"].toString());
    }

    if (obj.contains("children")) {
        QJsonArray children = obj["children"].toArray();
        for (int i = 0; i < children.size(); ++i) {
            QJsonObject child = children[i].toObject();
            extractNames(child, names);
        }
    }
}

QStringList getNames(const QJsonObject& root) {
    QStringList names;
    extractNames(root, names);
    return names;
}



Dataset<DatasetImpl> CrossSpeciesComparisonTreeData::createDataSet(const QString& guid /*= ""*/) const
{
    auto dataset = Dataset<DatasetImpl>(new CrossSpeciesComparisonTree(getName(), true, guid));
    return dataset;
}


void CrossSpeciesComparisonTreeData::setTreeDataRaw(QJsonObject jsonString)
{
    //sortJsonObject(jsonString);

    
    //qDebug() << "**************************************************";
    _data = jsonString;
    _leafNames.clear();
    _leafNames = getNames(_data);
    _leafNames.sort();
    //std::cout<< "Species names: " << _speciesNames.join(", ").toStdString() << std::endl;
    //qDebug() << "**************************************************";
}

void CrossSpeciesComparisonTreeData::setTreeLeafNamesRaw(QStringList jsonString)
{
    _leafNames = jsonString;
}

QJsonObject& CrossSpeciesComparisonTreeData::getTreeDataRaw()
{
    return _data;
}

QStringList& CrossSpeciesComparisonTreeData::getTreeLeafNamesRaw()
{
    return _leafNames;
}

QIcon CrossSpeciesComparisonTreeDataFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return Application::getIconFont("FontAwesome").getIcon("sitemap", color);
}

mv::plugin::RawData* CrossSpeciesComparisonTreeDataFactory::produce()
{
    return new CrossSpeciesComparisonTreeData(this);
}

void CrossSpeciesComparisonTree::init()
{
    _infoAction = QSharedPointer<InfoAction>::create(nullptr, *this);

    addAction(*_infoAction.get());

}

QIcon CrossSpeciesComparisonTree::getIcon(const QColor& color /*= Qt::black*/) const
{
    return Application::getIconFont("FontAwesome").getIcon("sitemap", color);
}

std::vector<std::uint32_t>& CrossSpeciesComparisonTree::getSelectionIndices()
{
    return getSelection<CrossSpeciesComparisonTree>()->indices;
}

void CrossSpeciesComparisonTree::setSelectionIndices(const std::vector<std::uint32_t>& indices)
{
}

bool CrossSpeciesComparisonTree::canSelect() const
{
    return false;
}

bool CrossSpeciesComparisonTree::canSelectAll() const
{
    return false;
}

bool CrossSpeciesComparisonTree::canSelectNone() const
{
    return false;
}

bool CrossSpeciesComparisonTree::canSelectInvert() const
{
    return false;
}

void CrossSpeciesComparisonTree::selectAll()
{
}

void CrossSpeciesComparisonTree::selectNone()
{
}

void CrossSpeciesComparisonTree::selectInvert()
{
}

void CrossSpeciesComparisonTree::setTreeData(QJsonObject jsonString)
{
    //qDebug() << "%%3ItsSetting3%%";
    getRawData<CrossSpeciesComparisonTreeData>()->setTreeDataRaw(jsonString);
    //qDebug()<< "jsonString"<<jsonString;
    //qDebug() << "%%3ItsSetting3%%";
    //getRawData<CrossSpeciesComparisonTreeData>()->changed();
}
void CrossSpeciesComparisonTree::setTreeLeafNames(QStringList jsonString)
{
    getRawData<CrossSpeciesComparisonTreeData>()->setTreeLeafNamesRaw(jsonString);
}
QJsonObject& CrossSpeciesComparisonTree::getTreeData()
{
    return  getRawData<CrossSpeciesComparisonTreeData>()->getTreeDataRaw();// TODO: insert return statement here
}

QStringList& CrossSpeciesComparisonTree::getTreeLeafNames()
{
    return  getRawData<CrossSpeciesComparisonTreeData>()->getTreeLeafNamesRaw();// TODO: insert return statement here
}

void CrossSpeciesComparisonTree::fromVariantMap(const QVariantMap& variantMap)
{
    /*DatasetImpl::fromVariantMap(variantMap);

    variantMapMustContain(variantMap, "DimensionNames");
    variantMapMustContain(variantMap, "Selection");

    // For backwards compatibility, check PluginVersion
    if (variantMap["PluginVersion"] == "No Version" && !variantMap["Full"].toBool())
    {
        makeSubsetOf(getParent()->getFullDataset<mv::DatasetImpl>());

        qWarning() << "[ManiVault deprecation warning]: This project was saved with an older ManiVault version (<1.0). "
            "Please save the project again to ensure compatibility with newer ManiVault versions. "
            "Future releases may not be able to load this projects otherwise. ";
    }

    // Load raw point data
    if (isFull())
        getRawData<PointData>()->fromVariantMap(variantMap);
    else
    {
        variantMapMustContain(variantMap, "Indices");

        const auto& indicesMap = variantMap["Indices"].toMap();

        indices.resize(indicesMap["Count"].toInt());

        populateDataBufferFromVariantMap(indicesMap["Raw"].toMap(), (char*)indices.data());
    }

    // Load dimension names
    QStringList dimensionNameList;
    std::vector<QString> dimensionNames;

    // Fetch dimension names from map
    const auto fetchDimensionNames = [&variantMap]() -> QStringList {
        QStringList dimensionNames;

        // Dimension names in byte array format
        QByteArray dimensionsByteArray;

        // Copy the dimension names raw data into the byte array
        dimensionsByteArray.resize(variantMap["DimensionNames"].toMap()["Size"].value<std::uint64_t>());
        populateDataBufferFromVariantMap(variantMap["DimensionNames"].toMap(), (char*)dimensionsByteArray.data());

        // Open input data stream
        QDataStream dimensionsDataStream(&dimensionsByteArray, QIODevice::ReadOnly);

        // Stream the data to the dimension names
        dimensionsDataStream >> dimensionNames;

        return dimensionNames;
        };

    if (variantMap["NumberOfDimensions"].toInt() > 1000)
        dimensionNameList = fetchDimensionNames();
    else
        dimensionNameList = variantMap["DimensionNames"].toStringList();

    if (dimensionNameList.size() == getNumDimensions())
    {
        for (const auto& dimensionName : dimensionNameList)
            dimensionNames.push_back(dimensionName);
    }
    else
    {
        for (std::uint32_t dimensionIndex = 0; dimensionIndex < getNumDimensions(); dimensionIndex++)
            dimensionNames.emplace_back(QString("Dim %1").arg(QString::number(dimensionIndex)));
    }

    setDimensionNames(dimensionNames);

    if (variantMap.contains("Dimensions")) {
        _dimensionsPickerAction->fromParentVariantMap(variantMap);
    }

    events().notifyDatasetDataChanged(this);

    // Handle saved selection
    if (isFull()) {
        const auto& selectionMap = variantMap["Selection"].toMap();

        const auto count = selectionMap["Count"].toInt();

        if (count > 0) {
            auto selectionSet = getSelection<Points>();

            selectionSet->indices.resize(count);

            populateDataBufferFromVariantMap(selectionMap["Raw"].toMap(), (char*)selectionSet->indices.data());

            events().notifyDatasetDataSelectionChanged(this);
        }
    }
  */
}

QVariantMap CrossSpeciesComparisonTree::toVariantMap() const
{
    
    QVariantMap variantMap;
    /*
    auto variantMap = DatasetImpl::toVariantMap();

    QStringList dimensionNames;
    QByteArray dimensionsByteArray;
    QDataStream dimensionsDataStream(&dimensionsByteArray, QIODevice::WriteOnly);

    if (getDimensionNames().size() == getNumDimensions()) {
        for (const auto& dimensionName : getDimensionNames())
            dimensionNames << dimensionName;
    }
    else {
        for (std::uint32_t dimensionIndex = 0; dimensionIndex < getNumDimensions(); dimensionIndex++)
            dimensionNames << QString("Dim %1").arg(QString::number(dimensionIndex));
    }

    if (dimensionNames.size() > 1000)
        dimensionsDataStream << dimensionNames;

    QVariantMap indices;

    indices["Count"] = QVariant::fromValue(this->indices.size());
    indices["Raw"] = rawDataToVariantMap((char*)this->indices.data(), this->indices.size() * sizeof(std::uint32_t), true);

    QVariantMap selection;

    if (isFull()) {
        auto selectionSet = getSelection<Points>();

        selection["Count"] = QVariant::fromValue(selectionSet->indices.size());
        selection["Raw"] = rawDataToVariantMap((char*)selectionSet->indices.data(), selectionSet->indices.size() * sizeof(std::uint32_t), true);
    }

    variantMap["Data"] = isFull() ? getRawData<PointData>()->toVariantMap() : QVariantMap();
    variantMap["NumberOfPoints"] = getNumPoints();
    variantMap["Indices"] = indices;
    variantMap["Selection"] = selection;
    variantMap["DimensionNames"] = (dimensionNames.size() > 1000) ? rawDataToVariantMap((char*)dimensionsByteArray.data(), dimensionsByteArray.size(), true) : QVariant::fromValue(dimensionNames);
    variantMap["NumberOfDimensions"] = getNumDimensions();
    variantMap["Dimensions"] = _dimensionsPickerAction->toVariantMap();
    */
    return variantMap;
}

