/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkSystemComponent.h"
#include <Clients/GenAIFrameworkSystemComponentConfiguration.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <Action/GenAIFrameworkLauncherScriptExecutor.h>
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

    AZStd::unique_ptr<GenAIFrameworkScriptExecutor> GenAIFrameworkSystemComponent::MakeScriptExecutor(
        [[maybe_unused]] const AIContext& aiContext)
    {
        return AZStd::make_unique<GenAIFrameworkLauncherScriptExecutor>();
    }

    void GenAIFrameworkSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        GenAIFrameworkLauncherScriptExecutor::Reflect(context);
        GenAIFrameworkSystemComponentConfiguration::Reflect(context);
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
                ->Event("GetActiveModelConfigurationsNames", &GenAIFrameworkRequestBus::Events::GetActiveModelConfigurationsNames)
                ->Event("GetActiveServiceProvidersNames", &GenAIFrameworkRequestBus::Events::GetActiveServiceProvidersNames);
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

        if (GenAIFrameworkActionInterface::Get() == nullptr)
        {
            GenAIFrameworkActionInterface::Register(&m_actionRequestHandler);
        }
    }

    GenAIFrameworkSystemComponent::~GenAIFrameworkSystemComponent()
    {
        if (GenAIFrameworkInterface::Get() == this)
        {
            GenAIFrameworkInterface::Unregister(this);
        }

        if (GenAIFrameworkActionInterface::Get() == &m_actionRequestHandler)
        {
            GenAIFrameworkActionInterface::Unregister(&m_actionRequestHandler);
        }
    }

    SystemRegistrationContext* GenAIFrameworkSystemComponent::GetSystemRegistrationContext()
    {
        return AZ::ReflectionEnvironment::GetReflectionManager()
            ? AZ::ReflectionEnvironment::GetReflectionManager()->GetReflectContext<SystemRegistrationContext>()
            : nullptr;
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetRegisteredComponentsNameAndComponentTypeId(
        const AZStd::set<AZ::Uuid>& registeredComponents)
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

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::
        GetRegisteredModelConfigurationsNameAndComponentTypeId()
    {
        auto modelConfigurations = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        return GetRegisteredComponentsNameAndComponentTypeId(modelConfigurations);
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetRegisteredServiceProvidersNameAndComponentTypeId()
    {
        auto registeredProviders = GetSystemRegistrationContext()->GetRegisteredServiceProviders();
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

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetActiveModelConfigurations()
    {
        return GetActiveComponents(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetActiveServiceProviders()
    {
        return GetActiveComponents(m_configuration.m_serviceProviders);
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewComponentEntity(
        const AZStd::string& name, const AZ::Uuid& componentTypeId, EntityIdToEntityMap& entities)
    {
        AZStd::shared_ptr<AZ::Entity> newEntity = nullptr;
        newEntity = AZStd::make_shared<AZ::Entity>(name);
        entities[newEntity->GetId()] = newEntity;
        newEntity->Init();
        const auto component = newEntity->CreateComponent(componentTypeId);
        newEntity->Activate();
        return component;
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewModelConfiguration(
        const AZStd::string& configurationName, const AZ::Uuid& componentTypeId)
    {
        auto* component = CreateNewComponentEntity(configurationName, componentTypeId, m_configuration.m_modelConfigurations);
        GenAIFrameworkNotificationBus::Broadcast(
            &GenAIFrameworkNotificationBus::Events::OnModelConfigurationAdded, component->GetEntity()->GetId());
        return component;
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewServiceProvider(
        const AZStd::string& providerName, const AZ::Uuid& componentTypeId)
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
        m_actionRequestHandler.Connect();

        ActivateEntities(m_configuration.m_serviceProviders);
        ActivateEntities(m_configuration.m_modelConfigurations);

        auto registeredGenerators = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        AZ::SerializeContext* serializeContext = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);
    }

    void GenAIFrameworkSystemComponent::Deactivate()
    {
        m_actionRequestHandler.Disconnect();
        GenAIFrameworkRequestBus::Handler::BusDisconnect();

        DeactivateEntities(m_configuration.m_serviceProviders);
        DeactivateEntities(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZStd::string> GenAIFrameworkSystemComponent::GetActiveModelConfigurationsNames() const
    {
        return GetActiveComponentsNames(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZStd::string> GenAIFrameworkSystemComponent::GetActiveServiceProvidersNames() const
    {
        return GetActiveComponentsNames(m_configuration.m_serviceProviders);
    }

    AZStd::vector<AZStd::string> GenAIFrameworkSystemComponent::GetActiveComponentsNames(const EntityIdToEntityMap& entities) const
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

} // namespace GenAIFramework
