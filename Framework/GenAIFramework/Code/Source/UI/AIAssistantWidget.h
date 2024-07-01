/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <GenAIFramework/GenAIFrameworkBus.h>
#include <GenAIFramework/UI/UIConversatonBus.h>
#include <UI/AgentConfigurationWidget/AgentConfigurationWidget.h>
#include <UI/NewChatWidget.h>

#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QWidget>
#endif

namespace Ui
{
    class AIAssistantUI;
}

namespace GenAIFramework
{
    class AIAssistantWidget : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit AIAssistantWidget(QWidget* parent = nullptr);

    private slots:
        void OnChatCreated(const QString& chatName, const QString& modelName, const QString& providerName, const QString& featureName);
        void OnChatClosed();

    protected:
        // QWidget overrides
        void closeEvent(QCloseEvent* event) override;
        void showEvent(QShowEvent* event) override;

        void CreateChat(const QString& chatName, const QString& modelName, const QString& providerName, const QString& featureName);

    private:
        Ui::AIAssistantUI* m_ui;
        AgentConfigurationWidget* m_agentConfigurationWidget;
        NewChatWidget* m_newChatWidget;
    };
} // namespace GenAIFramework
