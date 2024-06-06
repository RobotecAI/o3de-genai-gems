/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root
 * of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIAssistantWidget.h"
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

        AZ::SystemTickBus::QueueFunction(
            [this]()
            {
                UpdateModelAndProviderLists();
                RefreshDefaultModelConfiguration();
                GenAIFramework::GenAIFrameworkNotificationBus::Handler::BusConnect();
            });

        // listen to the provider and model configuration changes
        connect(m_ui->models, &QComboBox::textActivated, this, &AIAssistantWidget::OnModelConfigurationSelected);
        connect(m_ui->providers, &QComboBox::textActivated, this, &AIAssistantWidget::OnServiceProviderSelected);
        connect(m_ui->SendBtn, &QPushButton::clicked, this, &AIAssistantWidget::OnRequestButton);
        connect(m_ui->OptionsBtn, &QPushButton::clicked, this, &AIAssistantWidget::OnOptionsButton);
        connect(m_ui->actionResetHistory, &QAction::triggered, this, &AIAssistantWidget::OnResetAction);
    }

    AIAssistantWidget::~AIAssistantWidget()
    {
        if (GenAIFramework::GenAIFrameworkNotificationBus::Handler::BusIsConnected())
        {
            GenAIFramework::GenAIFrameworkNotificationBus::Handler::BusDisconnect();
        }
    }

    void AIAssistantWidget::RefreshDefaultModelConfiguration()
    {
        QSettings settings;
        const QString providerName = settings.value(SetServiceProvider).toString();
        const QString modelName = settings.value(SetModelConfiguration).toString();
        if (!providerName.isEmpty() && !modelName.isEmpty())
        {
            const int indexProvider = m_ui->providers->findText(providerName);
            const int indexModel = m_ui->models->findText(modelName);
            if (indexProvider >= 0 && indexModel >= 0)
            {
                m_ui->providers->setCurrentIndex(indexProvider);
                m_ui->models->setCurrentIndex(indexModel);
            }
            SetModelAndProvider(modelName, providerName);
        }
    }

    void SetToQSettings(const QString& providerName, const QString& modelName)
    {
        QSettings settings;
        settings.setValue(SetModelConfiguration, modelName);
        settings.setValue(SetServiceProvider, providerName);
    }

    void AIAssistantWidget::OnModelConfigurationSelected(const QString& modelName)
    {
        QString providerName = m_ui->providers->currentText();
        SetModelAndProvider(modelName, providerName);
        SetToQSettings(providerName, modelName);
    }

    void AIAssistantWidget::OnServiceProviderSelected(const QString& providerName)
    {
        QString modelName = m_ui->models->currentText();
        SetModelAndProvider(modelName, providerName);
        SetToQSettings(providerName, modelName);
    }

    void AIAssistantWidget::OnRequestButton()
    {
        AZStd::string modelInput = m_ui->textEdit->toPlainText().toStdString().c_str();
        UiAppendChatMessage(modelInput);
        constexpr bool isAssistantReply = true;
        UiAppendChatMessage("This PoC implementation is not connected to any assistant", isAssistantReply);
    }

    void AIAssistantWidget::OnOptionsButton()
    {
        m_optionsWidget->resize(this->size());
        m_optionsWidget->show();
    }

    void AIAssistantWidget::UpdateModelAndProviderLists()
    {
        m_ServiceProviderNameToId.clear();
        m_modelConfigurationNameToId.clear();
        m_ui->providers->clear();
        m_ui->models->clear();

        AZStd::vector<AZ::Component*> serviceProviders;
        GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
            serviceProviders, &GenAIFramework::GenAIFrameworkRequests::GetServiceProviders);
        for (auto* serviceProvider : serviceProviders)
        {
            if (serviceProvider->GetEntity() != nullptr)
            {
                AZ_Printf("AIAssistant", "Service Provider Name: %s", serviceProvider->GetEntity()->GetName().c_str());
                AZStd::string name = serviceProvider->GetEntity()->GetName();
                QString qName = QString::fromUtf8(name.c_str(), name.size());
                m_ServiceProviderNameToId[qName] = serviceProvider->GetEntityId();
                m_ui->providers->addItem(qName);
            }
        }

        AZStd::vector<AZ::Component*> modelConfigurations;
        GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
            modelConfigurations, &GenAIFramework::GenAIFrameworkRequests::GetModelConfigurations);

        for (auto* modelConfiguration : modelConfigurations)
        {
            if (modelConfiguration->GetEntity() != nullptr)
            {
                AZ_Printf("AIAssistant", "Model Configuration Name: %s", modelConfiguration->GetEntity()->GetName().c_str());
                AZStd::string name = modelConfiguration->GetEntity()->GetName();
                QString qName = QString::fromUtf8(name.c_str(), name.size());
                m_modelConfigurationNameToId[qName] = modelConfiguration->GetEntityId();
                m_ui->models->addItem(qName);
            }
        }
    }

    QString GetEntityName(AZ::EntityId entityId)
    {
        AZ::Entity* entity = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(entity, &AZ::ComponentApplicationBus::Events::FindEntity, entityId);
        if (entity != nullptr)
        {
            return QString::fromUtf8(entity->GetName().c_str(), entity->GetName().size());
        }
        return QString();
    }

    void AIAssistantWidget::OnServiceProviderAdded(const AZ::EntityId& serviceProviderId)
    {
        QString serviceProviderName = GetEntityName(serviceProviderId);
        m_ServiceProviderNameToId[serviceProviderName] = serviceProviderId;
        m_ui->providers->addItem(serviceProviderName);
    }

    void AIAssistantWidget::OnServiceProviderRemoved(const AZ::EntityId& serviceProviderId)
    {
        const QString serviceProviderName = GetEntityName(serviceProviderId);
        if (m_ServiceProviderNameToId.contains(serviceProviderName))
        {
            m_ServiceProviderNameToId.remove(serviceProviderName);
        }
        const int index = m_ui->providers->findText(serviceProviderName);
        if (index >= 0)
        {
            m_ui->providers->removeItem(index);
        }
    };

    void AIAssistantWidget::OnModelConfigurationAdded(const AZ::EntityId& modelConfigurationId)
    {
        QString modelConfigurationName = GetEntityName(modelConfigurationId);
        m_modelConfigurationNameToId[modelConfigurationName] = modelConfigurationId;
        m_ui->models->addItem(modelConfigurationName);
    };

    void AIAssistantWidget::OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId)
    {
        const QString modelConfigurationName = GetEntityName(modelConfigurationId);
        if (m_modelConfigurationNameToId.contains(modelConfigurationName))
        {
            m_modelConfigurationNameToId.remove(modelConfigurationName);
        }
        const int index = m_ui->models->findText(modelConfigurationName);
        if (index >= 0)
        {
            m_ui->models->removeItem(index);
        }
    };

    void AIAssistantWidget::SetModelAndProvider(const QString& modelName, const QString& providerName)
    {
        AZ_Assert(m_ServiceProviderNameToId.contains(providerName), "Provider name not found in the map");
        AZ_Assert(m_modelConfigurationNameToId.contains(modelName), "Model name not found in the map");
        if (m_ServiceProviderNameToId.contains(providerName) && m_modelConfigurationNameToId.contains(modelName))
        {
            AZ::EntityId selectedProviderId = m_ServiceProviderNameToId[providerName];
            AZ::EntityId selectedModelConfigurationId = m_modelConfigurationNameToId[modelName];
            GenAIFramework::AsyncRequestBus::Broadcast(
                &GenAIFramework::AsyncRequestBus::Events::SetProviderAndModel, selectedModelConfigurationId, selectedProviderId);
        }
    }

    void AIAssistantWidget::UiAppendChatMessage(const AZStd::string& message, bool response)
    {
        auto label = new QLabel(message.c_str());
        label->setWordWrap(true);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        if (response)
        {
            label->setStyleSheet("QLabel { background-color: #303030; margin-right: 100px; }");
        }
        else
        {
            label->setStyleSheet("QLabel { background-color: #202020; margin-left: 100px; }");
        }

        m_uiChatLayout->addWidget(label);
    }

    void AIAssistantWidget::UiClearMessages()
    {
        QLayoutItem* item;
        while ((item = m_uiChatLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
    }

    void AIAssistantWidget::OnResetAction()
    {
        // TODO: reset history
        UiClearMessages();
    }

    void AIAssistantWidget::closeEvent(QCloseEvent* event)
    {
        // TODO: reset history
        UiClearMessages();
        QMainWindow::closeEvent(event);
    }

    void AIAssistantWidget::showEvent(QShowEvent* event)
    {
        // TODO: reset history
        QMainWindow::showEvent(event);
    }
} // namespace GenAIFramework
