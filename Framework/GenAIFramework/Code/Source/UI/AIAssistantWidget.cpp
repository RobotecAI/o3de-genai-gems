/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIAssistantWidget.h"

#include <GenAIFramework/GenAIFrameworkEditorBus.h>
#include <GenAIFramework/GenAIFrameworkTypes.h>
#include <GenAIFramework/UI/UIConversatonBus.h>
#include <UI/ChatWidget.h>

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
        m_newChatWidget = new NewChatWidget(m_agentConfigurationWidget);

        connect(m_newChatWidget, &NewChatWidget::chatCreated, this, &AIAssistantWidget::OnChatCreated);

        m_ui->conversations->addTab(m_newChatWidget, "+");

        FeaturesConversationsStore storedChats;
        UIConversationsBus::BroadcastResult(storedChats, &UIConversationsBus::Events::GetStoredChats);

        for (const auto& chat : storedChats)
        {
            const auto& [modelName, providerName, featureName] = chat.second;

            CreateChat(
                QString::fromStdString(chat.first.c_str()),
                QString::fromStdString(modelName.c_str()),
                QString::fromStdString(providerName.c_str()),
                QString::fromStdString(featureName.c_str()));
        }
    }

    void AIAssistantWidget::OnChatCreated(
        const QString& chatName, const QString& modelName, const QString& providerName, const QString& featureName)
    {
        UIConversationsBus::Broadcast(
            &UIConversationsBus::Events::OnNewChatWidgetCreated,
            chatName.toStdString().c_str(),
            modelName.toStdString().c_str(),
            providerName.toStdString().c_str(),
            featureName.toStdString().c_str());

        CreateChat(chatName, modelName, providerName, featureName);
    }

    void AIAssistantWidget::CreateChat(
        const QString& chatName, const QString& modelName, const QString& providerName, const QString& featureName)
    {
        auto* newChatWidget = new ChatWidget(this, modelName, providerName, featureName);
        const auto tabIndex = m_ui->conversations->insertTab(0, newChatWidget, chatName);
        m_ui->conversations->setCurrentIndex(tabIndex);
        connect(newChatWidget, &ChatWidget::chatClosed, this, &AIAssistantWidget::OnChatClosed);

        auto interfaceEditor = GenAIFrameworkEditorInterface::Get();
        if (interfaceEditor)
        {
            interfaceEditor->SaveSystemConfiguration();
        }
    }

    void AIAssistantWidget::OnChatClosed()
    {
        const auto tabIndex = m_ui->conversations->currentIndex();
        QString tabName = m_ui->conversations->tabText(tabIndex);
        UIConversationsBus::Broadcast(&UIConversationsBus::Events::OnChatWidgetClosed, tabName.toStdString().c_str());

        m_ui->conversations->removeTab(tabIndex);

        auto interfaceEditor = GenAIFrameworkEditorInterface::Get();
        if (interfaceEditor)
        {
            interfaceEditor->SaveSystemConfiguration();
        }
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
