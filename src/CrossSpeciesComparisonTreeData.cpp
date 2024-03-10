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

    qDebug() << "**************************************************";

    std::cout << QJsonDocument(jsonString).toJson().toStdString() << std::endl;
    _data = jsonString;
    _speciesNames.clear();
    _speciesNames = getNames(_data);
    std::cout<< "Species names: " << _speciesNames.join(", ").toStdString() << std::endl;
    qDebug() << "**************************************************";
}

void CrossSpeciesComparisonTreeData::setTreeSpeciesNamesRaw(QStringList jsonString)
{
        _speciesNames = jsonString;
}

QJsonObject& CrossSpeciesComparisonTreeData::getTreeDataRaw()
{
    return _data;
}

QStringList& CrossSpeciesComparisonTreeData::getTreeSpeciesNamesRaw()
{
    return _speciesNames;
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
    qDebug() << "%%3ItsSetting3%%";
    getRawData<CrossSpeciesComparisonTreeData>()->setTreeDataRaw(jsonString);
    qDebug()<< "jsonString"<<jsonString;
    qDebug() << "%%3ItsSetting3%%";
    //getRawData<CrossSpeciesComparisonTreeData>()->changed();
    events().notifyDatasetDataChanged(this);
}
void CrossSpeciesComparisonTree::setTreeSpeciesNames(QStringList jsonString)
{
    getRawData<CrossSpeciesComparisonTreeData>()->setTreeSpeciesNamesRaw(jsonString);
}
QJsonObject& CrossSpeciesComparisonTree::getTreeData()
{
    return  getRawData<CrossSpeciesComparisonTreeData>()->getTreeDataRaw();// TODO: insert return statement here
}

QStringList& CrossSpeciesComparisonTree::getTreeSpeciesNames()
{
    return  getRawData<CrossSpeciesComparisonTreeData>()->getTreeSpeciesNamesRaw();// TODO: insert return statement here
}
