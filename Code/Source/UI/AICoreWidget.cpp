
#include "AICoreWidget.h"
#include "UI/RequesterTab.h"
#include <qwidget.h>

namespace AICore
{
    AICoreWidget::AICoreWidget(QWidget* parent)
        : QWidget(parent)
    {
        QVBoxLayout* verticalLayout = new QVBoxLayout(this);
        verticalLayout->setContentsMargins(0, 5, 0, 0);
        verticalLayout->setSpacing(0);

        m_tabs = new AzQtComponents::TabWidget(this);
        AzQtComponents::TabWidget::applySecondaryStyle(m_tabs, false);

        m_requesterTab = new RequesterTab();

        m_tabs->addTab(m_requesterTab, "Requester Configuration");

        verticalLayout->addWidget(m_tabs);
    }
} // namespace AICore