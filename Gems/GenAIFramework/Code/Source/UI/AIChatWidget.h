/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QWidget>
#endif

#include <AzCore/Component/Entity.h>
#include <AzCore/std/containers/unordered_map.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

#include "GenAIFrameworkWidget.h"

namespace Ui
{
    class AIAssistantUI;
}

namespace GenAIFramework
{
    class AIChatWidget
        : public QWidget
        , private GenAIFramework::GenAIFrameworkNotificationBus::Handler
    {
        Q_OBJECT
    public:
        explicit AIChatWidget(QWidget* parent = nullptr);
        ~AIChatWidget() override;

    //private slots:
    //    void OnModelConfigurationSelected(const QString& modelName);
    //    void OnServiceProviderSelected(const QString& providerName);
    //    void OnRequestButton();
    //    void OnOptionsButton();
    //    void OnResetAction();
    //    void OnNewChatAction();

    //protected:
    //    // QWidget overrides
    //    void closeEvent(QCloseEvent* event) override;
    //    void showEvent(QShowEvent* event) override;

    //private:
    //    // GenAIFramework::GenAIFrameworkNotificationBus::Handler
    //    void OnServiceProviderAdded(const AZ::EntityId& serviceProviderId) override;
    //    void OnServiceProviderRemoved(const AZ::EntityId& serviceProviderId) override;
    //    void OnModelConfigurationAdded(const AZ::EntityId& modelConfigurationId) override;
    //    void OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId) override;

    //    //! Refresh the list of service providers and model configurations, loads the default model and provider from QSettings.
    //    void RefreshDefaultModelConfiguration();

    //    //! Set the model and provider to the given names.
    //    //! @param modelName The name of the model configuration.
    //    //! @param providerName The name of the service provider.
    //    void SetModelAndProvider(const QString& modelName, const QString& providerName);

    //    void UiAppendChatMessage(const AZStd::string& message, bool response = false);
    //    void UiClearMessages();

    //    Ui::AIAssistantUI* m_ui;
    //    QVBoxLayout* m_uiChatLayout;
    //    GenAIFrameworkWidget* m_optionsWidget;

    //    QMap<QString, AZ::EntityId> m_ServiceProviderNameToId;
    //    QMap<QString, AZ::EntityId> m_modelConfigurationNameToId;
    };
} // namespace GenAIFramework
