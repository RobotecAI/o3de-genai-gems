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
#include <UI/AgentConfigurationWidget/AgentConfigurationWidget.h>

#include <AzCore/Component/Entity.h>

#include <QLabel>
#include <QMap>
#include <QVBoxLayout>
#include <QWidget>

#endif

namespace Ui
{
    class NewChatWidgetUI;
}

namespace GenAIFramework
{
    class NewChatWidget
        : public QWidget
        , private GenAIFramework::GenAIFrameworkNotificationBus::Handler
    {
        Q_OBJECT
    public:
        explicit NewChatWidget(AgentConfigurationWidget* agentConfigurationWidget, QWidget* parent = nullptr);
        void UpdateModelAndProviderLists();
        void UpdateFeaturesList();
        ~NewChatWidget() override;

    signals:
        // NOTE: For UI purposes these can be only strings, but given that this is connected in AIAssistantWidget, it could be useful to
        // provide more data on providers
        void chatCreated(const QString& chatName, const QString& modelName, const QString& providerName, const QString& featureName);

    private slots:
        void OnCreateButton();
        void OnAgentConfigurationButton();

    protected:
        // QWidget overrides
        void closeEvent(QCloseEvent* event) override;

    private:
        // GenAIFramework::GenAIFrameworkNotificationBus::Handler
        void OnServiceProviderAdded(const AZ::EntityId& serviceProviderId) override;
        void OnServiceProviderRemoved(const AZ::EntityId& serviceProviderId) override;
        void OnModelConfigurationAdded(const AZ::EntityId& modelConfigurationId) override;
        void OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId) override;

        //! Refresh the list of service providers and model configurations, loads the default model and provider from QSettings.
        void RefreshDefaultModelConfiguration();

        Ui::NewChatWidgetUI* m_ui;
        AgentConfigurationWidget* m_agentConfigurationWidget;

        QMap<QString, AZ::EntityId> m_ServiceProviderNameToId;
        QMap<QString, AZ::EntityId> m_modelConfigurationNameToId;
    };
} // namespace GenAIFramework
