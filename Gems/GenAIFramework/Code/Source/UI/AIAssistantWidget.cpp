/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIAssistantWidget.h"
#include "AIChatWidget.h"
#include "NewChatWidget.h"
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
#include <Source/UI/ui_AIAssistantWidget.h>

namespace GenAIFramework
{
    static constexpr const char SetModelConfiguration[] = "AIAssistant/Model";
    static constexpr const char SetServiceProvider[] = "AIAssistant/Provider";

    AIAssistantWidget::AIAssistantWidget(QWidget* parent)
        : QMainWindow(parent)
        , m_ui(new Ui::AIAssistantUI)

    {
        m_ui->setupUi(this);
        m_optionsWidget = new GenAIFrameworkWidget();

        connect(m_ui->actionNewChat, &QAction::triggered, this, &AIAssistantWidget::OnNewChatAction);
        connect(m_ui->actionConfigure, &QAction::triggered, this, &AIAssistantWidget::OnConfigureAction);
        connect(m_ui->conversations, &QTabWidget::tabCloseRequested, [this](int index)
            {
              m_ui->conversations->removeTab(index);
            });
    }

    AIAssistantWidget::~AIAssistantWidget()
    {
    }

    void AIAssistantWidget::OnConfigureAction()
    {
        //TODO: fix bug where the widget doesn't open after closing
        m_optionsWidget->resize(this->size());
        m_optionsWidget->show();
    }

    void AIAssistantWidget::OnNewChatAction()
    {
      QWidget * chatConfig = new NewChatWidget();
      chatConfig->show();
      //m_ui->conversations->addTab(new AIChatWidget(this), "New Chat");
    }

    void AIAssistantWidget::closeEvent(QCloseEvent* event)
    {
        // TODO: reset history
        QMainWindow::closeEvent(event);
    }

    void AIAssistantWidget::showEvent(QShowEvent* event)
    {
        // TODO: reset history
        QMainWindow::showEvent(event);
    }
} // namespace GenAIFramework
