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
#include <QMap>
#include <QVBoxLayout>
#include <QWidget>

#include <AzCore/Component/Entity.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

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
        explicit NewChatWidget(QWidget* parent = nullptr);
        void UpdateModelAndProviderLists();
        void UpdateFeaturesList();
        ~NewChatWidget() override;

    signals:
        // NOTE: For UI purposes these can be only strings, but given that this is connected in AIAssistantWidget, it could be useful to
        // provide more data on providers
        void chatCreated(const QString& chatName, const QString& modelName, const QString& providerName, const QString& featureName);

    private slots:
        void OnModelConfigurationSelected(const QString& modelName);
        void OnServiceProviderSelected(const QString& providerName);
        void OnChatNameChanged(const QString& chatName);
        void OnSaveButton();

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

        //! Set the model and provider to the given names.
        //! @param modelName The name of the model configuration.
        //! @param providerName The name of the service provider.
        void SetModelAndProvider(const QString& modelName, const QString& providerName);

        Ui::NewChatWidgetUI* m_ui;
        QVBoxLayout* m_uiChatLayout;
        QString m_chatName;
        QString m_modelName;
        QString m_providerName;

        QMap<QString, AZ::EntityId> m_ServiceProviderNameToId;
        QMap<QString, AZ::EntityId> m_modelConfigurationNameToId;
    };
} // namespace GenAIFramework
