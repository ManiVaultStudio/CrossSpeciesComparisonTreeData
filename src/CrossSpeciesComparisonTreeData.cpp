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
    /*WidgetAction::fromVariantMap(variantMap);

    const auto dataMap = variantMap["Data"].toMap();

    variantMapMustContain(dataMap, "IndicesRawData");
    variantMapMustContain(dataMap, "NumberOfIndices");

    // Packed indices for all clusters
    QVector<std::uint32_t> packedIndices;

    packedIndices.resize(dataMap["NumberOfIndices"].toInt());

    // Convert raw data to indices
    populateDataBufferFromVariantMap(dataMap["IndicesRawData"].toMap(), (char*)packedIndices.data());

    if (dataMap.contains("ClustersRawData")) {
        QByteArray clustersByteArray;

        QDataStream clustersDataStream(&clustersByteArray, QIODevice::ReadOnly);

        const auto clustersRawDataSize = dataMap["ClustersRawDataSize"].toInt();

        clustersByteArray.resize(clustersRawDataSize);

        populateDataBufferFromVariantMap(dataMap["ClustersRawData"].toMap(), (char*)clustersByteArray.data());

        QVariantList clusters;

        clustersDataStream >> clusters;

        _clusters.resize(clusters.count());

        long clusterIndex = 0;

        for (const auto& clusterVariant : clusters) {
            const auto clusterMap = clusterVariant.toMap();

            auto& cluster = _clusters[clusterIndex];

            cluster.setName(clusterMap["Name"].toString());
            cluster.setId(clusterMap["ID"].toString());
            cluster.setColor(clusterMap["Color"].toString());

            const auto globalIndicesOffset = clusterMap["GlobalIndicesOffset"].toInt();
            const auto numberOfIndices = clusterMap["NumberOfIndices"].toInt();

            cluster.getIndices() = std::vector<std::uint32_t>(packedIndices.begin() + globalIndicesOffset, packedIndices.begin() + globalIndicesOffset + numberOfIndices);

            ++clusterIndex;
        }
    }

    // For backwards compatibility
    if (dataMap.contains("Clusters")) {
        const auto clustersList = dataMap["Clusters"].toList();

        _clusters.resize(clustersList.count());

        for (const auto& clusterVariant : clustersList) {
            const auto clusterMap = clusterVariant.toMap();
            const auto clusterIndex = clustersList.indexOf(clusterMap);

            auto& cluster = _clusters[clusterIndex];

            cluster.setName(clusterMap["Name"].toString());
            cluster.setId(clusterMap["ID"].toString());
            cluster.setColor(clusterMap["Color"].toString());

            const auto globalIndicesOffset = clusterMap["GlobalIndicesOffset"].toInt();
            const auto numberOfIndices = clusterMap["NumberOfIndices"].toInt();

            cluster.getIndices() = std::vector<std::uint32_t>(packedIndices.begin() + globalIndicesOffset, packedIndices.begin() + globalIndicesOffset + numberOfIndices);
        }
    }
    */
}

QVariantMap CrossSpeciesComparisonTreeData::toVariantMap() const
{
    auto variantMap = WidgetAction::toVariantMap();
    /*
    std::vector<std::uint32_t> indices;

    for (const auto& cluster : _clusters)
        indices.insert(indices.end(), cluster.getIndices().begin(), cluster.getIndices().end());

    QVariantMap indicesRawData = rawDataToVariantMap((char*)indices.data(), indices.size() * sizeof(std::uint32_t), true);

    std::size_t globalIndicesOffset = 0;

    QVariantList clusters;

    clusters.reserve(_clusters.count());

    for (const auto& cluster : _clusters) {
        const auto numberOfIndicesInCluster = cluster.getIndices().size();

        clusters.push_back(QVariantMap({
            { "Name", cluster.getName() },
            { "ID", cluster.getId() },
            { "Color", cluster.getColor() },
            { "GlobalIndicesOffset", QVariant::fromValue(globalIndicesOffset) },
            { "NumberOfIndices", QVariant::fromValue(numberOfIndicesInCluster) }
            }));

        globalIndicesOffset += numberOfIndicesInCluster;
    }

    // https://stackoverflow.com/questions/19537186/serializing-qvariant-through-qdatastream

    QByteArray clustersByteArray;
    QDataStream clustersDataStream(&clustersByteArray, QIODevice::WriteOnly);

    clustersDataStream << clusters;

    QVariantMap clustersRawData = rawDataToVariantMap((char*)clustersByteArray.data(), clustersByteArray.size(), true);

    variantMap.insert({
        { "ClustersRawData", clustersRawData },
        { "ClustersRawDataSize", clustersByteArray.size() },
        { "IndicesRawData", indicesRawData },
        { "NumberOfIndices", QVariant::fromValue(indices.size()) }
        });
        */
    return variantMap;
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
    DatasetImpl::fromVariantMap(variantMap);

    getRawData<CrossSpeciesComparisonTreeData>()->fromVariantMap(variantMap);

    events().notifyDatasetDataChanged(this);
  
}

QVariantMap CrossSpeciesComparisonTree::toVariantMap() const
{
    auto variantMap = DatasetImpl::toVariantMap();

    variantMap["Data"] = getRawData<CrossSpeciesComparisonTreeData>()->toVariantMap();

    return variantMap;
}

