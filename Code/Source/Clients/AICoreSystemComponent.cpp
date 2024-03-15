/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AICoreSystemComponent.h"
#include "AzCore/Component/Entity.h"
#include "AzCore/std/smart_ptr/make_shared.h"
#include "AzCore/std/smart_ptr/shared_ptr.h"
#include "Clients/AICoreSystemComponentConfiguration.h"

#include <AICore/AICoreTypeIds.h>
#include <Action/AICoreLauncherScriptExecutor.h>
#include <AzCore/Component/ComponentApplication.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/utility/move.h>

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    AZ_COMPONENT_IMPL(AICoreSystemComponent, "AICoreSystemComponent", AICoreSystemComponentTypeId);

    AZStd::unique_ptr<AICoreScriptExecutor> AICoreSystemComponent::MakeScriptExecutor([[maybe_unused]] const AIContext& aiContext)
    {
        return AZStd::make_unique<AICoreLauncherScriptExecutor>();
    }

    void AICoreSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        AICoreLauncherScriptExecutor::Reflect(context);
        AICoreSystemComponentConfiguration::Reflect(context);
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AICoreSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void AICoreSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("AICoreService"));
    }

    void AICoreSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("AICoreService"));
    }

    void AICoreSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("HTTPRequestorService"));
    }

    void AICoreSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    AICoreSystemComponent::AICoreSystemComponent()
    {
        if (AICoreInterface::Get() == nullptr)
        {
            AICoreInterface::Register(this);
        }

        if (AICoreActionInterface::Get() == nullptr)
        {
            AICoreActionInterface::Register(&m_actionRequestHandler);
        }
    }

    AICoreSystemComponent::~AICoreSystemComponent()
    {
        if (AICoreInterface::Get() == this)
        {
            AICoreInterface::Unregister(this);
        }

        if (AICoreActionInterface::Get() == &m_actionRequestHandler)
        {
            AICoreActionInterface::Unregister(&m_actionRequestHandler);
        }
    }

    AICoreSystemRegistrationContext* AICoreSystemComponent::GetSystemRegistrationContext()
    {
        return AZ::ReflectionEnvironment::GetReflectionManager()
            ? AZ::ReflectionEnvironment::GetReflectionManager()->GetReflectContext<AICoreSystemRegistrationContext>()
            : nullptr;
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> AICoreSystemComponent::GetRegisteredComponentsNameAndComponentTypeId(
        AZStd::vector<AZ::Uuid> componentTypeIds)
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

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> AICoreSystemComponent::GetRegisteredModelConfigurationsNameAndComponentTypeId()
    {
        auto modelConfigurations = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        return GetRegisteredComponentsNameAndComponentTypeId(modelConfigurations);
    }

    AZStd::vector<AZStd::pair<AZStd::string, AZ::Uuid>> AICoreSystemComponent::GetRegisteredServiceRequestersNameAndComponentTypeId()
    {
        auto registeredRequesters = GetSystemRegistrationContext()->GetRegisteredServiceRequesters();
        return GetRegisteredComponentsNameAndComponentTypeId(registeredRequesters);
    }

    AZStd::vector<AZ::Component*> AICoreSystemComponent::GetActiveComponents(AZStd::vector<AZStd::shared_ptr<AZ::Entity>> entities)
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

    AZStd::vector<AZ::Component*> AICoreSystemComponent::GetActiveModelConfigurations()
    {
        return GetActiveComponents(m_configuration.m_modelConfigurations);
    }

    AZStd::vector<AZ::Component*> AICoreSystemComponent::GetActiveServiceRequesters()
    {
        return GetActiveComponents(m_configuration.m_serviceRequesters);
    }

    AZ::Component* AICoreSystemComponent::CreateNewComponentEntity(
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

    AZ::Component* AICoreSystemComponent::CreateNewModelConfiguration(
        const AZStd::string& configurationName, const AZ::Uuid& componentTypeId)
    {
        return CreateNewComponentEntity(configurationName, componentTypeId, m_configuration.m_modelConfigurations);
    }

    AZ::Component* AICoreSystemComponent::CreateNewServiceRequester(const AZStd::string& requesterName, const AZ::Uuid& componentTypeId)
    {
        return CreateNewComponentEntity(requesterName, componentTypeId, m_configuration.m_serviceRequesters);
    }

    void AICoreSystemComponent::RemoveComponent(AZ::Component* component)
    {
        if (component)
        {
            AZStd::shared_ptr<AZ::Entity> entity;
            entity.reset(component->GetEntity());
            if (entity)
            {
                entity->Deactivate();

                auto it = AZStd::find(m_configuration.m_serviceRequesters.begin(), m_configuration.m_serviceRequesters.end(), entity);
                if (it != m_configuration.m_serviceRequesters.end())
                {
                    m_configuration.m_serviceRequesters.erase(it);
                }

                it = AZStd::find(m_configuration.m_modelConfigurations.begin(), m_configuration.m_modelConfigurations.end(), entity);
                if (it != m_configuration.m_modelConfigurations.end())
                {
                    m_configuration.m_modelConfigurations.erase(it);
                }
            }
        }
    }

    void AICoreSystemComponent::ActivateEntity(AZStd::shared_ptr<AZ::Entity> entity)
    {
        if (entity)
        {
            entity->Activate();
        }
    }

    void AICoreSystemComponent::DeactivateEntity(AZStd::shared_ptr<AZ::Entity> entity)
    {
        if (entity)
        {
            entity->Deactivate();
        }
    }

    void AICoreSystemComponent::InitEntities(const AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        for (auto entity : entities)
        {
            if (entity)
            {
                entity->Init();
            }
        }
    }

    void AICoreSystemComponent::ActivateEntities(const AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        for (auto entity : entities)
        {
            if (entity)
            {
                entity->Activate();
            }
        }
    }

    void AICoreSystemComponent::DeactivateEntities(const AZStd::vector<AZStd::shared_ptr<AZ::Entity>>& entities)
    {
        for (auto entity : entities)
        {
            if (entity)
            {
                entity->Deactivate();
            }
        }
    }

    void AICoreSystemComponent::Init()
    {
        // Enable the AWS SDK
        Aws::SDKOptions options;
        Aws::InitAPI(options);
        
        auto result = m_settingsRegistryManager.LoadSystemConfiguration();
        if (result.has_value())
        {
            m_configuration = *result;
        }
        InitEntities(m_configuration.m_serviceRequesters);
        InitEntities(m_configuration.m_modelConfigurations);
    }

    void AICoreSystemComponent::Activate()
    {
        AICoreRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
        m_actionRequestHandler.Connect();

        ActivateEntities(m_configuration.m_serviceRequesters);
        ActivateEntities(m_configuration.m_modelConfigurations);

        auto registeredGenerators = GetSystemRegistrationContext()->GetRegisteredModelConfigurations();
        AZ::SerializeContext* serializeContext = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(serializeContext, &AZ::ComponentApplicationBus::Events::GetSerializeContext);
    }

    void AICoreSystemComponent::Deactivate()
    {
        m_actionRequestHandler.Disconnect();
        AZ::TickBus::Handler::BusDisconnect();
        AICoreRequestBus::Handler::BusDisconnect();

        DeactivateEntities(m_configuration.m_serviceRequesters);
        DeactivateEntities(m_configuration.m_modelConfigurations);
    }

    void AICoreSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }
} // namespace AICore
