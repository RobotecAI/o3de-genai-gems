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
#include <Source/UI/ui_NewChatWidget.h>

namespace GenAIFramework
{
    static constexpr const char SetModelConfiguration[] = "AIAssistant/Model";
    static constexpr const char SetServiceProvider[] = "AIAssistant/Provider";

    NewChatWidget::NewChatWidget(QWidget* parent)
        : QWidget(parent)
        , m_ui(new Ui::NewChatWidgetUI)

    {
        m_ui->setupUi(this);
        AZ::SystemTickBus::QueueFunction(
            [this]()
            {
                UpdateModelAndProviderLists();
                UpdateFeaturesList();
                RefreshDefaultModelConfiguration();
                GenAIFramework::GenAIFrameworkNotificationBus::Handler::BusConnect();
            });
        connect(m_ui->models, &QComboBox::textActivated, this, &NewChatWidget::OnModelConfigurationSelected);
        connect(m_ui->providers, &QComboBox::textActivated, this, &NewChatWidget::OnServiceProviderSelected);
        connect(m_ui->chatName, &QLineEdit::textChanged, this, &NewChatWidget::OnChatNameChanged);
        connect(m_ui->saveButton, &QPushButton::clicked, this, &NewChatWidget::OnSaveButton);
    }

    NewChatWidget::~NewChatWidget()
    {
        if (GenAIFramework::GenAIFrameworkNotificationBus::Handler::BusIsConnected())
        {
            GenAIFramework::GenAIFrameworkNotificationBus::Handler::BusDisconnect();
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

    void SetToQSettings(const QString& providerName, const QString& modelName)
    {
        QSettings settings;
        settings.setValue(SetModelConfiguration, modelName);
        settings.setValue(SetServiceProvider, providerName);
    }

    void NewChatWidget::OnChatNameChanged(const QString& chatName)
    {
        m_chatName = chatName;
    }

    void NewChatWidget::OnSaveButton()
    {
        auto featureName = m_ui->features->currentText();
        emit chatCreated(m_chatName, m_modelName, m_providerName, featureName);
    }

    void NewChatWidget::closeEvent([[maybe_unused]] QCloseEvent* event)
    {
        this->hide();
    }

    void NewChatWidget::OnServiceProviderAdded(const AZ::EntityId& serviceProviderId)
    {
        QString serviceProviderName = GetEntityName(serviceProviderId);
        m_ServiceProviderNameToId[serviceProviderName] = serviceProviderId;
        m_ui->providers->addItem(serviceProviderName);
    }

    void NewChatWidget::OnServiceProviderRemoved(const AZ::EntityId& serviceProviderId)
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

    void NewChatWidget::OnModelConfigurationAdded(const AZ::EntityId& modelConfigurationId)
    {
        QString modelConfigurationName = GetEntityName(modelConfigurationId);
        m_modelConfigurationNameToId[modelConfigurationName] = modelConfigurationId;
        m_ui->models->addItem(modelConfigurationName);
    };

    void NewChatWidget::OnModelConfigurationRemoved(const AZ::EntityId& modelConfigurationId)
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

    void NewChatWidget::OnModelConfigurationSelected(const QString& modelName)
    {
        QString providerName = m_ui->providers->currentText();
        SetModelAndProvider(modelName, providerName);
        SetToQSettings(providerName, modelName);
        m_modelName = modelName;
    }

    void NewChatWidget::OnServiceProviderSelected(const QString& providerName)
    {
        QString modelName = m_ui->models->currentText();
        SetModelAndProvider(modelName, providerName);
        SetToQSettings(providerName, modelName);
        m_providerName = providerName;
    }

    void NewChatWidget::SetModelAndProvider(const QString& modelName, const QString& providerName)
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

    void NewChatWidget::UpdateModelAndProviderLists()
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

        m_providerName = m_ui->providers->currentText();

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

        m_modelName = m_ui->models->currentText();
    }

    void NewChatWidget::UpdateFeaturesList()
    {
        auto registeredFeatures = GenAIFrameworkInterface::Get()->GetSystemRegistrationContext()->GetFeatureNamesAndUuids();
        for (const auto& feature : registeredFeatures)
        {
            m_ui->features->addItem(QString::fromUtf8(feature.first.c_str(), feature.first.size()));
        }
    }

    void NewChatWidget::RefreshDefaultModelConfiguration()
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

} // namespace GenAIFramework
