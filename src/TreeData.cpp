#include "TreeData.h"

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

Q_PLUGIN_METADATA(IID "nl.BioVault.TreeData")

using namespace mv;



TreeData::~TreeData(void)
{

}

void TreeData::init()
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

Dataset<DatasetImpl> TreeData::createDataSet(const QString& guid /*= ""*/) const
{
    auto dataset = Dataset<DatasetImpl>(new Tree(getName(), true, guid));
    return dataset;
}

void TreeData::setData(QJsonObject jsonString)
{
    sortJsonObject(jsonString);
    std::cout << QJsonDocument(jsonString).toJson().toStdString() << std::endl;
    _data = jsonString;
    _speciesNames.clear();
    _speciesNames = getNames(_data);

}

QJsonObject& TreeData::getData()
{
    return _data;
}

QStringList& TreeData::getSpeciesNames()
{
    return _speciesNames;
}

QIcon TreeDataFactory::getIcon(const QColor& color /*= Qt::black*/) const
{
    return Application::getIconFont("FontAwesome").getIcon("sitemap", color);
}

mv::plugin::RawData* TreeDataFactory::produce()
{
    return new TreeData(this);
}

QIcon Tree::getIcon(const QColor& color /*= Qt::black*/) const
{
    return Application::getIconFont("FontAwesome").getIcon("sitemap", color);
}

std::vector<std::uint32_t>& Tree::getSelectionIndices()
{
    return getSelection<Tree>()->indices;
}

void Tree::setSelectionIndices(const std::vector<std::uint32_t>& indices)
{
}

bool Tree::canSelect() const
{
    return false;
}

bool Tree::canSelectAll() const
{
    return false;
}

bool Tree::canSelectNone() const
{
    return false;
}

bool Tree::canSelectInvert() const
{
    return false;
}

void Tree::selectAll()
{
}

void Tree::selectNone()
{
}

void Tree::selectInvert()
{
}

void Tree::setData(QJsonObject jsonString)
{
    getRawData<TreeData>()->setData(jsonString);
}
QJsonObject& Tree::getData()
{
    return  getRawData<TreeData>()->getData();// TODO: insert return statement here
}

QStringList& Tree::getSpeciesNames()
{
    return  getRawData<TreeData>()->getSpeciesNames();// TODO: insert return statement here
}
