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
    AIAssistantWidget::AIAssistantWidget(QWidget* parent)
        : QMainWindow(parent)
        , m_ui(new Ui::AIAssistantUI)

    {
        m_ui->setupUi(this);
        m_agentConfigurationWidget = new AgentConfigurationWidget();
        m_newChatWidget = new NewChatWidget();

        connect(m_ui->actionConfigure, &QAction::triggered, this, &AIAssistantWidget::OnConfigureAction);
        connect(m_newChatWidget, &NewChatWidget::chatCreated, this, &AIAssistantWidget::OnChatCreated);

        m_ui->conversations->addTab(m_newChatWidget, "+");
    }

    void AIAssistantWidget::OnConfigureAction()
    {
        m_agentConfigurationWidget->resize(this->size());
        m_agentConfigurationWidget->show();
    }

    void AIAssistantWidget::OnChatCreated(const QString& chatName, const QString& modelName, const QString& providerName)
    {
        auto* newChatWidget = new AIChatWidget(this, modelName, providerName);
        const auto tabIndex = m_ui->conversations->insertTab(0, newChatWidget, chatName);
        m_ui->conversations->setCurrentIndex(tabIndex);
        connect(newChatWidget, &AIChatWidget::chatClosed, this, &AIAssistantWidget::OnChatClosed);
    }

    void AIAssistantWidget::OnChatClosed()
    {
        const auto tabIndex = m_ui->conversations->currentIndex();
        m_ui->conversations->removeTab(tabIndex);
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
