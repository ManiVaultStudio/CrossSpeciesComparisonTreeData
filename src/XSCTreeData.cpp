#include "XSCTreeData.h"
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

Q_PLUGIN_METADATA(IID "nl.BioVault.XSCTreeData")

using namespace mv;



XSCTreeData::~XSCTreeData(void)
{

}

void XSCTreeData::fromVariantMap(const QVariantMap& variantMap)
{
    RawData::fromVariantMap(variantMap);

    if (variantMap.contains("CSV:TreeData")) {
        QJsonDocument doc = QJsonDocument::fromJson(variantMap["CSV:TreeData"].toByteArray());
        setTreeDataRaw(doc.object());
    }
}

QVariantMap XSCTreeData::toVariantMap() const
{ 
    QVariantMap variantMap = RawData::toVariantMap();
    
    QJsonDocument doc(_data);

    variantMap["CSV:TreeData"] = doc.toJson();

    return variantMap;
}

void XSCTreeData::init()
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



Dataset<DatasetImpl> XSCTreeData::createDataSet(const QString& guid /*= ""*/) const
{
    auto dataset = Dataset<DatasetImpl>(new XSCTree(getName(), true, guid));
    return dataset;
}


void XSCTreeData::setTreeDataRaw(QJsonObject jsonString)
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

void XSCTreeData::setTreeLeafNamesRaw(QStringList jsonString)
{
    _leafNames = jsonString;
}

QJsonObject& XSCTreeData::getTreeDataRaw()
{
    return _data;
}

QStringList& XSCTreeData::getTreeLeafNamesRaw()
{
    return _leafNames;
}

XSCTreeDataFactory::XSCTreeDataFactory() 
{
    setIconByName("sitemap");
}

mv::plugin::RawData* XSCTreeDataFactory::produce()
{
    return new XSCTreeData(this);
}

void XSCTree::init()
{
    _infoAction = QSharedPointer<InfoAction>::create(nullptr, *this);

    addAction(*_infoAction.get());

}

std::vector<std::uint32_t>& XSCTree::getSelectionIndices()
{
    return getSelection<XSCTree>()->indices;
}

void XSCTree::setSelectionIndices(const std::vector<std::uint32_t>& indices)
{
}

bool XSCTree::canSelect() const
{
    return false;
}

bool XSCTree::canSelectAll() const
{
    return false;
}

bool XSCTree::canSelectNone() const
{
    return false;
}

bool XSCTree::canSelectInvert() const
{
    return false;
}

void XSCTree::selectAll()
{
}

void XSCTree::selectNone()
{
}

void XSCTree::selectInvert()
{
}

void XSCTree::setTreeData(QJsonObject jsonString)
{
    //qDebug() << "%%3ItsSetting3%%";
    getRawData<XSCTreeData>()->setTreeDataRaw(jsonString);
    //qDebug()<< "jsonString"<<jsonString;
    //qDebug() << "%%3ItsSetting3%%";
    //getRawData<XSCTreeData>()->changed();
}
void XSCTree::setTreeLeafNames(QStringList jsonString)
{
    getRawData<XSCTreeData>()->setTreeLeafNamesRaw(jsonString);
}
QJsonObject& XSCTree::getTreeData()
{
    return  getRawData<XSCTreeData>()->getTreeDataRaw();// TODO: insert return statement here
}

QStringList& XSCTree::getTreeLeafNames()
{
    return  getRawData<XSCTreeData>()->getTreeLeafNamesRaw();// TODO: insert return statement here
}

void XSCTree::fromVariantMap(const QVariantMap& variantMap)
{
    DatasetImpl::fromVariantMap(variantMap);

    getRawData<XSCTreeData>()->fromParentVariantMap(variantMap);
    events().notifyDatasetDataSelectionChanged(this);


}

QVariantMap XSCTree::toVariantMap() const
{
    auto variantMap = DatasetImpl::toVariantMap();

    getRawData<XSCTreeData>()->insertIntoVariantMap(variantMap);

    return variantMap;
}

