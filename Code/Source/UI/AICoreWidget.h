
#pragma once

#include "UI/RequesterTab.h"
#if !defined(Q_MOC_RUN)
#include "RequesterTab.h"
#include <AzQtComponents/Components/Widgets/TabWidget.h>
#include <qobjectdefs.h>
#include <qwidget.h>
#endif

namespace AICore
{
    class AICoreWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit AICoreWidget(QWidget* parent = nullptr);

    private:
        AzQtComponents::TabWidget* m_tabs;
        RequesterTab* m_requesterTab;
    };
} // namespace AICore