#include "InfoAction.h"

#include "event/Event.h"

using namespace mv;
using namespace mv::gui;

InfoAction::InfoAction(QObject* parent, CrossSpeciesComparisonTree& clusters) :
    GroupAction(parent, "Group", true),
    _clusters(&clusters),
    _treeInfoAction(this, "CrossSpeciesComparisonTree info"),
    _leafInfoAction(this, "Leaf info")
{
    setText("CrossSpeciesComparisonTree JSON Info");


    _treeInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);
    _leafInfoAction.setDefaultWidgetFlags(StringAction::TextEdit);

    addAction(&_leafInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });
    addAction(&_treeInfoAction, -1, [this](WidgetAction* action, QWidget* widget) -> void {
        auto textEdit = widget->findChild<QTextEdit*>("LineEdit");

        if (textEdit)
            textEdit->setReadOnly(true);

        });



    const auto updateActions = [this]() -> void {
        if (!_clusters.isValid())
            return;


        _treeInfoAction.setString(QJsonDocument(_clusters->getTreeData()).toJson());
        QString textleaf = _clusters->getTreeSpeciesNames().join("\n");
        _leafInfoAction.setString(textleaf);

        qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

        qDebug() << QJsonDocument(_clusters->getTreeData()).toJson();
        qDebug() << "Species names: " << textleaf ;
        qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

    };

    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetAdded));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataChanged));
    _eventListener.addSupportedEventType(static_cast<std::uint32_t>(EventType::DatasetDataSelectionChanged));


    _eventListener.registerDataEventByType(CrossSpeciesComparisonTreeType, [this, updateActions](mv::DatasetEvent* dataEvent) {
        if (!_clusters.isValid())
            return;

        if (dataEvent->getDataset() != _clusters)
            return;

        switch (dataEvent->getType()) {
            case EventType::DatasetAdded:
            case EventType::DatasetDataChanged:
            case EventType::DatasetDataSelectionChanged:
            {
                updateActions();
                break;
            }

            default:
                break;
        }
    });

    updateActions();
}