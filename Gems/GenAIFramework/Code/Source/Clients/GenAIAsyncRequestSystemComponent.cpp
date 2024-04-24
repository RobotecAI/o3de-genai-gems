/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "GenAIAsyncRequestSystemComponent.h"
#include <AzCore/Component/Entity.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>
#include <GenAIFramework/GenAIFrameworkBus.h>

namespace GenAIFramework
{
    AZ_COMPONENT_IMPL(GenAIAsyncRequestSystemComponent, "GenAIAsyncRequestSystemComponent", GenAIAsyncRequestSystemComponentTypeId);

    void GenAIAsyncRequestSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GenAIAsyncRequestSystemComponent, AZ::Component>()->Version(0);
        }
        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<AsyncRequestBus>("asyncRequestBus")
                ->Attribute(AZ::Script::Attributes::Category, "AI")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "ai")
                ->Event("SetServiceProviderByName", &AsyncRequestBus::Events::SetServiceProviderByName)
                ->Event("SetModelConfigurationByName", &AsyncRequestBus::Events::SetModelConfigurationByName)
                ->Event("GetActiveServiceProviderName", &AsyncRequestBus::Events::GetActiveServiceProviderName)
                ->Event("GetActiveModelConfigurationName", &AsyncRequestBus::Events::GetActiveModelConfigurationName)
                ->Event("SendPromptToLLM", &AsyncRequestBus::Events::SendPromptToLLM)
                ->Event("IsResponseReady", &AsyncRequestBus::Events::IsResponseReady)
                ->Event("GetResponse", &AsyncRequestBus::Events::GetResponse)
                ->Event("ResetModelHistory", &AsyncRequestBus::Events::ResetModelHistory)
                ->Event("EnableModelHistory", &AsyncRequestBus::Events::EnableModelHistory);
        }
    }

    GenAIAsyncRequestSystemComponent::GenAIAsyncRequestSystemComponent()
    {
        if (AsyncRequestBusInterface::Get() == nullptr)
        {
            AsyncRequestBusInterface::Register(this);
        }
    }

    GenAIAsyncRequestSystemComponent::~GenAIAsyncRequestSystemComponent()
    {
        if (AsyncRequestBusInterface::Get() == this)
        {
            AsyncRequestBusInterface::Unregister(this);
        }
    }

    void GenAIAsyncRequestSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GenAIAsyncRequestService"));
    }

    void GenAIAsyncRequestSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GenAIAsyncRequestService"));
    }
    void GenAIAsyncRequestSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("GenAIFrameworkService"));
    }

    void GenAIAsyncRequestSystemComponent::Activate()
    {
        AsyncRequestBus::Handler::BusConnect();
    }

    void GenAIAsyncRequestSystemComponent::Deactivate()
    {
        AsyncRequestBus::Handler::BusDisconnect();
    }

    void GenAIAsyncRequestSystemComponent::SetProviderAndModel(AZ::EntityId modelConfigurationId, AZ::EntityId serviceProviderId)
    {
        m_modelConfigurationId = modelConfigurationId;
        m_serviceProviderId = serviceProviderId;

        AZ_Printf("AiAssistantEditorSystemComponent", "Using model: %s", modelConfigurationId.ToString().c_str());
        AZ_Printf("AiAssistantEditorSystemComponent", "Using service: %s", serviceProviderId.ToString().c_str());
    }

    bool GenAIAsyncRequestSystemComponent::SetServiceProviderByName(const AZStd::string& providerName)
    {
        AZStd::vector<AZ::Component*> activeServiceProviders;
        GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
            activeServiceProviders, &GenAIFramework::GenAIFrameworkRequests::GetServiceProviders);
        return SetEntityIdByName(activeServiceProviders, providerName, m_serviceProviderId);
    }

    bool GenAIAsyncRequestSystemComponent::SetModelConfigurationByName(const AZStd::string& modelConfigurationName)
    {
        AZStd::vector<AZ::Component*> activeModelConfigurations;
        GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
            activeModelConfigurations, &GenAIFramework::GenAIFrameworkRequests::GetModelConfigurations);
        return SetEntityIdByName(activeModelConfigurations, modelConfigurationName, m_modelConfigurationId);
    }

    AZStd::string GenAIAsyncRequestSystemComponent::GetActiveServiceProviderName()
    {
        if (!m_serviceProviderId.IsValid())
        {
            AZ_Warning("GenAIAsyncRequestSystemComponent", false, "No service provider selected.");
            return {};
        }

        AZStd::vector<AZ::Component*> activeServiceProviders;
        GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
            activeServiceProviders, &GenAIFramework::GenAIFrameworkRequests::GetServiceProviders);
        return GetEntityName(activeServiceProviders, m_serviceProviderId);
    }

    AZStd::string GenAIAsyncRequestSystemComponent::GetActiveModelConfigurationName()
    {
        if (!m_modelConfigurationId.IsValid())
        {
            AZ_Warning("GenAIAsyncRequestSystemComponent", false, "No model configuration selected.");
            return {};
        }

        AZStd::vector<AZ::Component*> activeModelConfigurations;
        GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
            activeModelConfigurations, &GenAIFramework::GenAIFrameworkRequests::GetModelConfigurations);
        return GetEntityName(activeModelConfigurations, m_modelConfigurationId);
    }

    bool GenAIAsyncRequestSystemComponent::SetEntityIdByName(
        const AZStd::vector<AZ::Component*>& components, const AZStd::string& entityName, AZ::EntityId& entityId)
    {
        for (const auto& component : components)
        {
            if (component->GetEntity()->GetName() == entityName)
            {
                entityId = component->GetEntityId();
                return true;
            }
        }
        return false;
    }

    AZStd::string GenAIAsyncRequestSystemComponent::GetEntityName(const AZStd::vector<AZ::Component*>& components, AZ::EntityId& entityId)
    {
        for (const auto& component : components)
        {
            if (component->GetEntityId() == entityId)
            {
                return component->GetEntity()->GetName();
            }
        }
        return {};
    }

    AZ::Uuid GenAIAsyncRequestSystemComponent::SendPromptToLLM(const AZStd::string& prompt)
    {
        if (!m_modelConfigurationId.IsValid())
        {
            AZStd::vector<AZ::Component*> activeModelConfigurations;
            GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
                activeModelConfigurations, &GenAIFramework::GenAIFrameworkRequests::GetModelConfigurations);
            if (!activeModelConfigurations.empty())
            {
                m_modelConfigurationId = activeModelConfigurations.front()->GetEntityId();
                AZ_Warning(
                    "GenAIAsyncRequestSystemComponent",
                    false,
                    "No model configuration selected. Using the first available model configuration: %s",
                    m_modelConfigurationId.ToString().c_str())
            }
        }

        if (!m_serviceProviderId.IsValid())
        {
            AZStd::vector<AZ::Component*> activeServiceProviders;
            GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
                activeServiceProviders, &GenAIFramework::GenAIFrameworkRequests::GetServiceProviders);
            if (!activeServiceProviders.empty())
            {
                m_serviceProviderId = activeServiceProviders.front()->GetEntityId();
                AZ_Warning(
                    "GenAIAsyncRequestSystemComponent",
                    false,
                    "No model configuration selected. Using the first available service provider: %s",
                    m_serviceProviderId.ToString().c_str())
            }
        }

        AZ::Uuid promptId = AZ::Uuid::CreateRandom();
        GenAIFramework::ModelAPIPrompt modelPrompt;
        modelPrompt.push_back(AZStd::any(prompt));

        GenAIFramework::ModelAPIRequest preparedRequest;
        GenAIFramework::AIModelRequestBus::EventResult(
            preparedRequest, m_modelConfigurationId, &GenAIFramework::AIModelRequestBus::Events::PrepareRequest, modelPrompt);

        auto callback = [this, promptId](GenAIFramework::ModelAPIProviderResponse outcome)
        {
            if (!outcome.IsSuccess())
            {
                AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
                m_promptResponses[promptId] = "Error: " + outcome.GetError();
                AZ_Warning("AiAssistantEditorSystemComponent", false, "Request returned with an error: %s", outcome.GetError().c_str());
                return;
            }

            GenAIFramework::ModelAPIResponse extractedResponse;
            GenAIFramework::AIModelRequestBus::EventResult(
                extractedResponse, m_modelConfigurationId, &GenAIFramework::AIModelRequestBus::Events::ExtractResult, outcome.GetValue());
            if (extractedResponse.IsSuccess())
            {
                AZStd::string response = "";
                for (const auto& elem : extractedResponse.GetValue())
                {
                    response += AZStd::any_cast<AZStd::string>(elem);
                }
                AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
                m_promptResponses[promptId] = response;
            }
            else
            {
                AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
                AZStd::string result = "Error: " + extractedResponse.GetError();
                AZ_Warning("AiAssistantEditorSystemComponent", false, "Cannot get a response from the model: %s", result.c_str());
                m_promptResponses[promptId] = result;
            }
        };

        GenAIFramework::AIServiceProviderBus::Event(
            m_serviceProviderId, &GenAIFramework::AIServiceProviderBus::Events::SendRequest, preparedRequest, callback);

        return promptId;
    }

    bool GenAIAsyncRequestSystemComponent::IsResponseReady(AZ::Uuid promptId)
    {
        AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
        bool isReady = m_promptResponses.contains(promptId);
        return isReady;
    }

    AZStd::string GenAIAsyncRequestSystemComponent::GetResponse(AZ::Uuid promptId)
    {
        AZStd::string response;
        if (IsResponseReady(promptId))
        {
            AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
            response = m_promptResponses[promptId];
            m_promptResponses.erase(promptId);
        }
        return response;
    }

    void GenAIAsyncRequestSystemComponent::ResetModelHistory()
    {
        GenAIFramework::AIModelRequestBus::Event(m_modelConfigurationId, &GenAIFramework::AIModelRequestBus::Events::ResetModelHistory);
    }

    void GenAIAsyncRequestSystemComponent::EnableModelHistory(bool enableHistory)
    {
        GenAIFramework::AIModelRequestBus::Event(
            m_modelConfigurationId, &GenAIFramework::AIModelRequestBus::Events::EnableModelHistory, enableHistory);
    }

} // namespace GenAIFramework
