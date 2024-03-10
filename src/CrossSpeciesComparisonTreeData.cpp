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

QJsonObject sortJsonObject(QJsonObject obj) {
    if (obj.contains("children")) {
        QJsonArray childrenArray = obj["children"].toArray();
        std::vector<QJsonObject> children;

        for (const auto& child : childrenArray) {
            QJsonObject sortedChild = sortJsonObject(child.toObject());
            children.push_back(sortedChild);
        }

        std::sort(children.begin(), children.end(), [](const QJsonObject& a, const QJsonObject& b) {
            QString aName = a.contains("name") ? a["name"].toString() : "";
            QString bName = b.contains("name") ? b["name"].toString() : "";
            if (aName.isEmpty() && bName.isEmpty())
                return false;
            else if (aName.isEmpty())
                return true;
            else if (bName.isEmpty())
                return false;
            else if (aName.at(0) != bName.at(0))
                return aName.at(0) < bName.at(0); // change here
            else
                return aName < bName;
            });

        QJsonArray sortedChildrenArray;
        for (const auto& child : children) {
            sortedChildrenArray.push_back(child);
        }

        obj["children"] = sortedChildrenArray;
    }

    return obj;
}

Dataset<DatasetImpl> CrossSpeciesComparisonTreeData::createDataSet(const QString& guid /*= ""*/) const
{
    auto dataset = Dataset<DatasetImpl>(new CrossSpeciesComparisonTree(getName(), true, guid));
    return dataset;
}

void CrossSpeciesComparisonTreeData::setData(QJsonObject jsonString)
{
    sortJsonObject(jsonString);
    std::cout << QJsonDocument(jsonString).toJson().toStdString() << std::endl;
    _data = jsonString;
    _speciesNames.clear();
    _speciesNames = getNames(_data);

}

QJsonObject& CrossSpeciesComparisonTreeData::getData()
{
    return _data;
}

QStringList& CrossSpeciesComparisonTreeData::getSpeciesNames()
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

void CrossSpeciesComparisonTree::setData(QJsonObject jsonString)
{
    getRawData<CrossSpeciesComparisonTreeData>()->setData(jsonString);
}
QJsonObject& CrossSpeciesComparisonTree::getData()
{
    return  getRawData<CrossSpeciesComparisonTreeData>()->getData();// TODO: insert return statement here
}

QStringList& CrossSpeciesComparisonTree::getSpeciesNames()
{
    return  getRawData<CrossSpeciesComparisonTreeData>()->getSpeciesNames();// TODO: insert return statement here
}
