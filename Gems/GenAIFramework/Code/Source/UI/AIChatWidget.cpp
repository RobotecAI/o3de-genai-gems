/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIChatWidget.h"
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>

#include <AzCore/Component/ComponentApplicationBus.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIFramework/Communication/AsyncRequestBus.h>
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <Source/UI/ui_AIChatWidget.h>

namespace GenAIFramework
{
    AIChatWidget::AIChatWidget(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::AIChatWidgetUI)

    {
        m_ui->setupUi(this);
        m_optionsWidget = new GenAIFrameworkWidget();

        m_uiChatLayout = new QVBoxLayout();
        m_uiChatLayout->setAlignment(Qt::AlignBottom);
        m_ui->scrollArea->setWidgetResizable(true);
        m_ui->scrollArea->widget()->setLayout(m_uiChatLayout);
        m_ui->scrollArea->setStyleSheet("QLabel { border-radius: 15px; padding: 15px; }");
        m_ui->scrollArea->verticalScrollBar()->connect(
            m_ui->scrollArea->verticalScrollBar(),
            &QScrollBar::rangeChanged,
            [this]()
            {
                m_ui->scrollArea->verticalScrollBar()->setValue(m_ui->scrollArea->verticalScrollBar()->maximum());
            });

  }
  
  AIChatWidget::~AIChatWidget()
  {
  }


}
