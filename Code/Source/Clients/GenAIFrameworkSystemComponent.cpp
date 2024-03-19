/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIFrameworkSystemComponent.h"
#include "AzCore/Component/Entity.h"
#include "AzCore/std/smart_ptr/make_shared.h"
#include "AzCore/std/smart_ptr/shared_ptr.h"
#include "Clients/GenAIFrameworkSystemComponentConfiguration.h"

#include <GenAIFramework/GenAIFrameworkTypeIds.h>
#include <Action/GenAIFrameworkLauncherScriptExecutor.h>
#include <AzCore/Component/ComponentApplication.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/utility/move.h>

namespace GenAIFramework
{
    AZ_COMPONENT_IMPL(GenAIFrameworkSystemComponent, "GenAIFrameworkSystemComponent", GenAIFrameworkSystemComponentTypeId);

    AZStd::unique_ptr<GenAIFrameworkScriptExecutor> GenAIFrameworkSystemComponent::MakeScriptExecutor([[maybe_unused]] const AIContext& aiContext)
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

    GenAIFrameworkSystemRegistrationContext* GenAIFrameworkSystemComponent::GetSystemRegistrationContext()
    {
        return AZ::ReflectionEnvironment::GetReflectionManager()
            ? AZ::ReflectionEnvironment::GetReflectionManager()->GetReflectContext<GenAIFrameworkSystemRegistrationContext>()
            : nullptr;
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetRegisteredComponentsNameAndComponentTypeId(
        const AZStd::vector<AZ::Uuid>& componentTypeIds)
    {
        AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> result;
        auto registeredComponents = componentTypeIds;

        AZ::SerializeContext* serializeContext = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        for (auto& generator : registeredComponents)
        {
            auto classData = serializeContext->FindClassData(generator);
            auto serializedName = classData->m_name;
            if (classData->m_editData)
            {
                serializedName = classData->m_editData->m_name;
            }
            result.push_back({ serializedName, generator });
        }
        return result;
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetRegisteredModelConfigurationsNameAndComponentTypeId()
    {
        auto modelConfigurations = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        return GetRegisteredComponentsNameAndComponentTypeId(modelConfigurations);
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> GenAIFrameworkSystemComponent::GetRegisteredServiceRequestersNameAndComponentTypeId()
    {
        auto registeredRequesters = GetSystemRegistrationContext()->GetRegisteredServiceRequesters();
        return GetRegisteredComponentsNameAndComponentTypeId(registeredRequesters);
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetActiveComponents(AZStd::vector<AZStd::shared_ptr<AZ::Entity>> entities)
    {
        AZStd::vector<AZ::Component*> result;
        for (auto entity : entities)
        {
            if (entity)
            {
                auto components = entity->GetComponents();
                result.insert(result.end(), components.begin(), components.end());
            }
        }
        return result;
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetActiveModelConfigurations()
    {
        return GetActiveComponents(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZ::Component*> GenAIFrameworkSystemComponent::GetActiveServiceRequesters()
    {
        return GetActiveComponents(m_configuration.m_serviceRequesters);
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewComponentEntity(
        const AZStd::string& name, const AZ::Uuid& componentTypeId, AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        AZStd::shared_ptr<AZ::Entity> newEntity = nullptr;
        newEntity = AZStd::make_shared<AZ::Entity>(name);
        entities.push_back(newEntity);
        newEntity->Init();
        auto component = newEntity->CreateComponent(componentTypeId);
        newEntity->Activate();
        return component;
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewModelConfiguration(
        const AZStd::string& configurationName, const AZ::Uuid& componentTypeId)
    {
        return CreateNewComponentEntity(configurationName, componentTypeId, m_configuration.m_modelConfigurations);
    }

    AZ::Component* GenAIFrameworkSystemComponent::CreateNewServiceRequester(const AZStd::string& requesterName, const AZ::Uuid& componentTypeId)
    {
        return CreateNewComponentEntity(requesterName, componentTypeId, m_configuration.m_serviceRequesters);
    }

    void GenAIFrameworkSystemComponent::RemoveComponent(AZ::Component* component)
    {
        if (component)
        {
            AZStd::shared_ptr<AZ::Entity> entity;
            entity.reset(component->GetEntity());
            if (entity)
            {
                entity->Deactivate();

                if (auto it = AZStd::find(m_configuration.m_serviceRequesters.begin(), m_configuration.m_serviceRequesters.end(), entity);
                    it != m_configuration.m_serviceRequesters.end())
                {
                    m_configuration.m_serviceRequesters.erase(it);
                }
                if (auto it =
                        AZStd::find(m_configuration.m_modelConfigurations.begin(), m_configuration.m_modelConfigurations.end(), entity);
                    it != m_configuration.m_modelConfigurations.end())
                {
                    m_configuration.m_modelConfigurations.erase(it);
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

    void GenAIFrameworkSystemComponent::InitEntities(const AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        for (auto entity : entities)
        {
            if (entity)
            {
                entity->Init();
            }
        }
    }

    void GenAIFrameworkSystemComponent::ActivateEntities(const AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        for (auto entity : entities)
        {
            if (entity)
            {
                entity->Activate();
            }
        }
    }

    void GenAIFrameworkSystemComponent::DeactivateEntities(const AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        for (auto entity : entities)
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
        InitEntities(m_configuration.m_serviceRequesters);
        InitEntities(m_configuration.m_modelConfigurations);
    }

    void GenAIFrameworkSystemComponent::Activate()
    {
        AZ_Printf("GenAIFrameworkSystemComponent", "Activating GenAIFrameworkSystemComponent with %d service requesters and %d model configurations\n",
            m_configuration.m_serviceRequesters.size(), m_configuration.m_modelConfigurations.size());
        GenAIFrameworkRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
        m_actionRequestHandler.Connect();

        ActivateEntities(m_configuration.m_serviceRequesters);
        ActivateEntities(m_configuration.m_modelConfigurations);

        auto registeredGenerators = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        AZ::SerializeContext* serializeContext = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);
    }

    void GenAIFrameworkSystemComponent::Deactivate()
    {
        m_actionRequestHandler.Disconnect();
        AZ::TickBus::Handler::BusDisconnect();
        GenAIFrameworkRequestBus::Handler::BusDisconnect();

        DeactivateEntities(m_configuration.m_serviceRequesters);
        DeactivateEntities(m_configuration.m_modelConfigurations);
    }

    void GenAIFrameworkSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }
} // namespace GenAIFramework
