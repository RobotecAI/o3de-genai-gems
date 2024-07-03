/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkSystemComponent.h"
#include <Clients/GenAIFrameworkSystemComponentConfiguration.h>
#include <GenAIFramework/Feature/CppFeatureBase.h>
#include <GenAIFramework/Feature/FeatureBase.h>
#include <GenAIFramework/Feature/PythonFeatureBase.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <GenAIFramework/UI/UIConversationBus.h>
#include <O3DEAssistantFeature/O3DEAssistantFeature.h>

#include <AzCore/Component/ComponentApplication.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/EntityId.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/smart_ptr/make_shared.h>

namespace GenAIFramework
{
    AZ_COMPONENT_IMPL(GenAIFrameworkSystemComponent, "GenAIFrameworkSystemComponent", GenAIFrameworkSystemComponentTypeId);

    void GenAIFrameworkSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        FeatureBase::Reflect(context);
        CppFeatureBase::Reflect(context);
        PythonFeatureBase::Reflect(context);
        GenAIFrameworkSystemComponentConfiguration::Reflect(context);

        O3DEAssistantFeature::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIFrameworkSystemComponent, AZ::Component>()->Version(0);
        }
        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<GenAIFrameworkRequestBus>("GenAIFrameworkRequestBus")
                ->Attribute(AZ::Script::Attributes::Category, "GenAIFrameworkRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "ai")
                ->Event("GetModelConfigurationNames", &GenAIFrameworkRequestBus::Events::GetModelConfigurationNames)
                ->Event("GetServiceProviderNames", &GenAIFrameworkRequestBus::Events::GetServiceProviderNames);
        }
    }

    void GenAIFrameworkSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIFrameworkService"));
    }

    void GenAIFrameworkSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIFrameworkService"));
    }

    void GenAIFrameworkSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("HTTPRequestorService"));
    }

    void GenAIFrameworkSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    GenAIFrameworkSystemComponent::GenAIFrameworkSystemComponent()
    {
        if (GenAIFrameworkInterface::Get() == nullptr)
        {
            GenAIFrameworkInterface::Register(this);
        }
    }

    GenAIFrameworkSystemComponent::~GenAIFrameworkSystemComponent()
    {
        if (GenAIFrameworkInterface::Get() == this)
        {
            GenAIFrameworkInterface::Unregister(this);
        }
    }

    SystemRegistrationContext* GenAIFrameworkSystemComponent::GetSystemRegistrationContext() const
    {
        return AZ::ReflectionEnvironment::GetReflectionManager()
            ? AZ::ReflectionEnvironment::GetReflectionManager()->GetReflectContext<SystemRegistrationContext>()
            : nullptr;
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetRegisteredComponentsNameAndComponentTypeId(
        const AZStd::unordered_set<AZ::Uuid>& registeredComponents) const
    {
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> result;

        AZ::SerializeContext* serializeContext = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        for (auto& generator : registeredComponents)
        {
            const auto classData = serializeContext->FindClassData(generator);
            auto serializedName = classData->m_name;
            if (classData->m_editData)
            {
                serializedName = classData->m_editData->m_name;
            }
            result.emplace_back(AZStd::make_pair(serializedName, generator));
        }
        return result;
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetModelConfigurationNamesAndComponentTypeIds() const
    {
        const auto& modelConfigurations = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        return GetRegisteredComponentsNameAndComponentTypeId(modelConfigurations);
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetServiceProviderNamesAndComponentTypeIds() const
    {
        const auto& registeredProviders = GetSystemRegistrationContext()->GetRegisteredServiceProviders();
        return GetRegisteredComponentsNameAndComponentTypeId(registeredProviders);
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetActiveComponents(const EntityIdToEntityMap& entities) const
    {
        AZStd::vector<AZ::Component*> result;
        for (const auto& [_, entity] : entities)
        {
            if (entity)
            {
                const auto components = entity->GetComponents();
                result.insert(result.end(), components.begin(), components.end());
            }
        }
        return result;
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetModelConfigurations() const
    {
        return GetActiveComponents(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetServiceProviders() const
    {
        return GetActiveComponents(m_configuration.m_serviceProviders);
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewComponentEntity(
        const AZStd::string& name, AZ::Uuid componentTypeId, EntityIdToEntityMap& entities)
    {
        AZStd::shared_ptr<AZ::Entity> newEntity = nullptr;
        newEntity = AZStd::make_shared<AZ::Entity>(name);
        entities[newEntity->GetId()] = newEntity;
        newEntity->Init();
        const auto component = newEntity->CreateComponent(componentTypeId);
        newEntity->Activate();
        return component;
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateModelConfiguration(const AZStd::string& configurationName, AZ::Uuid componentTypeId)
    {
        auto* component = CreateNewComponentEntity(configurationName, componentTypeId, m_configuration.m_modelConfigurations);
        GenAIFrameworkNotificationBus::Broadcast(
            &GenAIFrameworkNotificationBus::Events::OnModelConfigurationAdded, component->GetEntity()->GetId());
        return component;
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateServiceProvider(const AZStd::string& providerName, AZ::Uuid componentTypeId)
    {
        auto* component = CreateNewComponentEntity(providerName, componentTypeId, m_configuration.m_serviceProviders);
        GenAIFrameworkNotificationBus::Broadcast(
            &GenAIFrameworkNotificationBus::Events::OnServiceProviderAdded, component->GetEntity()->GetId());
        return component;
    }

    void GenAIFrameworkSystemComponent::RemoveComponent(AZ::Component* component)
    {
        if (component)
        {
            AZ::EntityId entityId = component->GetEntity()->GetId();
            if (entityId.IsValid())
            {
                component->GetEntity()->Deactivate();

                if (m_configuration.m_serviceProviders.contains(entityId))
                {
                    GenAIFrameworkNotificationBus::Broadcast(&GenAIFrameworkNotificationBus::Events::OnServiceProviderRemoved, entityId);
                    m_configuration.m_serviceProviders.erase(entityId);
                }
                if (m_configuration.m_modelConfigurations.contains(entityId))
                {
                    GenAIFrameworkNotificationBus::Broadcast(&GenAIFrameworkNotificationBus::Events::OnModelConfigurationRemoved, entityId);
                    m_configuration.m_modelConfigurations.erase(entityId);
                }
            }
        }
    }

    void GenAIFrameworkSystemComponent::ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity)
    {
        if (entity)
        {
            entity->Activate();
        }
    }

    void GenAIFrameworkSystemComponent::DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity)
    {
        if (entity)
        {
            entity->Deactivate();
        }
    }

    void GenAIFrameworkSystemComponent::InitEntities(const EntityIdToEntityMap& entities)
    {
        for (auto& [_, entity] : entities)
        {
            if (entity)
            {
                entity->Init();
            }
        }
    }

    void GenAIFrameworkSystemComponent::ActivateEntities(const EntityIdToEntityMap& entities)
    {
        for (auto& [_, entity] : entities)
        {
            if (entity)
            {
                entity->Activate();
            }
        }
    }

    void GenAIFrameworkSystemComponent::DeactivateEntities(const EntityIdToEntityMap& entities)
    {
        for (auto& [_, entity] : entities)
        {
            if (entity)
            {
                entity->Deactivate();
            }
        }
    }

    void GenAIFrameworkSystemComponent::Init()
    {
        auto result = m_settingsRegistryManager.LoadSystemConfiguration();
        if (result.has_value())
        {
            m_configuration = *result;
        }
        InitEntities(m_configuration.m_serviceProviders);
        InitEntities(m_configuration.m_modelConfigurations);

        for (const auto& [entityId, _] : m_configuration.m_serviceProviders)
        {
            GenAIFrameworkNotificationBus::Broadcast(&GenAIFrameworkNotificationBus::Events::OnServiceProviderAdded, entityId);
        }
        for (const auto& [entityId, _] : m_configuration.m_modelConfigurations)
        {
            GenAIFrameworkNotificationBus::Broadcast(&GenAIFrameworkNotificationBus::Events::OnModelConfigurationAdded, entityId);
        }
    }

    void GenAIFrameworkSystemComponent::Activate()
    {
        AZ_Printf(
            "GenAIFrameworkSystemComponent",
            "Activating GenAIFrameworkSystemComponent with %d service providers and %d model configurations\n",
            m_configuration.m_serviceProviders.size(),
            m_configuration.m_modelConfigurations.size());

        GenAIFrameworkRequestBus::Handler::BusConnect();
        UIConversationsBus::Handler::BusConnect();

        ActivateEntities(m_configuration.m_serviceProviders);
        ActivateEntities(m_configuration.m_modelConfigurations);

        AZ::SerializeContext* serializeContext = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);
    }

    void GenAIFrameworkSystemComponent::Deactivate()
    {
        UIConversationsBus::Handler::BusDisconnect();
        GenAIFrameworkRequestBus::Handler::BusDisconnect();

        DeactivateEntities(m_configuration.m_serviceProviders);
        DeactivateEntities(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZStd::string> GenAIFrameworkSystemComponent::GetModelConfigurationNames() const
    {
        return GetRegisteredComponentNames(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZStd::string> GenAIFrameworkSystemComponent::GetServiceProviderNames() const
    {
        return GetRegisteredComponentNames(m_configuration.m_serviceProviders);
    }

    AZStd::vector<AZStd::string> GenAIFrameworkSystemComponent::GetRegisteredComponentNames(const EntityIdToEntityMap& entities) const
    {
        auto activeComponents = GetActiveComponents(entities);
        AZStd::vector<AZStd::string> result;
        AZStd::transform(
            activeComponents.begin(),
            activeComponents.end(),
            AZStd::back_inserter(result),
            [](AZ::Component* component)
            {
                return component->GetEntity()->GetName();
            });

        return result;
    }

    AZ::EntityId GenAIFrameworkSystemComponent::GetEntityIdByName(const AZStd::string& name, const EntityIdToEntityMap& entities) const
    {
        for (const auto& [entityId, entity] : entities)
        {
            if (entity && entity->GetName() == name)
            {
                return entityId;
            }
        }
        return AZ::EntityId();
    }

    AZ::Outcome<AZ::u64, void> GenAIFrameworkSystemComponent::CreateAIAgent(
        const AZStd::string& serviceProviderName, const AZStd::string& modelModelConfigurationName)
    {
        // Find service provider and model configuration
        AZ::EntityId serviceProviderId = GetEntityIdByName(serviceProviderName, m_configuration.m_serviceProviders);
        AZ::EntityId modelConfigurationId = GetEntityIdByName(modelModelConfigurationName, m_configuration.m_modelConfigurations);

        if (!serviceProviderId.IsValid() || !modelConfigurationId.IsValid())
        {
            return AZ::Failure();
        }

        // Generate a unique id for the model agent
        auto currentTime = AZStd::chrono::system_clock::now();
        AZ::u32 idTime = static_cast<AZ::u64>(currentTime.time_since_epoch().count());
        AZ::u64 id = m_agents.size() << 32 | idTime;

        m_agents[id] = AZStd::make_shared<AIAgent>(serviceProviderId, modelConfigurationId, id);

        return AZ::Success(id);
    }

    bool GenAIFrameworkSystemComponent::RemoveAIAgent(AZ::u64 agentId)
    {
        return (m_agents.erase(agentId) > 0);
    }

    AZ::Outcome<AZ::u64, void> GenAIFrameworkSystemComponent::CreateNewFeatureConversation(
        const AZStd::string& serviceProviderName, const AZStd::string& modelModelConfigurationName, const AZStd::string& featureName)
    {
        const auto& features = GetSystemRegistrationContext()->GetFeatureNamesAndUuids();
        const auto featureUuid = features.find(featureName);
        if (featureUuid == features.end())
        {
            return AZ::Failure();
        }

        const auto agentId = CreateAIAgent(serviceProviderName, modelModelConfigurationName);
        if (!agentId.IsSuccess())
        {
            return AZ::Failure();
        }

        // The system is currently limited to one feature (connected to one agent) per one conversation. Hence, the same id is used.
        auto conversationId = agentId;
        auto feature = GetSystemRegistrationContext()->CreateFeature(featureUuid->second, agentId.GetValue(), conversationId.GetValue());
        m_featureConversations[conversationId.GetValue()] = feature;

        return conversationId;
    }

    bool GenAIFrameworkSystemComponent::RemoveFeatureConversation(AZ::u64 featureConversationId)
    {
        return RemoveAIAgent(featureConversationId) ? m_featureConversations.erase(featureConversationId) > 0 : false;
    }

    void GenAIFrameworkSystemComponent::OnNewChatWidgetCreated(
        [[maybe_unused]] const AZStd::string& chatName,
        [[maybe_unused]] const AZStd::string& modelName,
        [[maybe_unused]] const AZStd::string& providerName,
        [[maybe_unused]] const AZStd::string& featureName)
    {
        m_configuration.m_FeaturesConversationsMap[chatName] = FeatureTuple{ modelName, providerName, featureName };
    }

    void GenAIFrameworkSystemComponent::OnChatWidgetClosed(const AZStd::string& chatName)
    {
        m_configuration.m_FeaturesConversationsMap.erase(chatName);
    }

    FeaturesConversationsMap GenAIFrameworkSystemComponent::GetStoredChats()
    {
        return m_configuration.m_FeaturesConversationsMap;
    }

} // namespace GenAIFramework
