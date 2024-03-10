#pragma once

#include "Dataset.h"
#include "CrossSpeciesComparisonTreeData.h"


#include "actions/StringAction.h"
#include "event/EventListener.h"

namespace mv {
    class CoreInterface;
}

using namespace mv;
using namespace mv::gui;
using namespace mv::util;


class InfoAction : public GroupAction
{
    Q_OBJECT

public:


    InfoAction(QObject* parent, CrossSpeciesComparisonTree& clusters);

public: // Action getters

    StringAction& getTreeInfoAction() { return _treeInfoAction; }
    StringAction& getLeafInfoAction() { return _leafInfoAction; }

protected:
    Dataset<CrossSpeciesComparisonTree>       _clusters;                  /** Clusters dataset smart pointer */
    StringAction    _treeInfoAction;
    StringAction    _leafInfoAction;

    mv::EventListener     _eventListener;             /** Listen to HDPS events */
};