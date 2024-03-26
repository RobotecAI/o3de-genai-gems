#pragma once

#include "GenAIAsyncRequestSystemComponent.h"
#include <AzCore/Component/Entity.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceRequesterBus.h>
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
                ->Event("SendPromptToLLM", &AsyncRequestBus::Events::SendPromptToLLM)
                ->Event("IsResponseReady", &AsyncRequestBus::Events::IsResponseReady)
                ->Event("GetResponse", &AsyncRequestBus::Events::GetResponse);
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

    void GenAIAsyncRequestSystemComponent::SetRequestorAndModel(AZ::EntityId selectedModelConfigurationId, AZ::EntityId selectedRequestorId)
    {
        m_selectedModelConfigurationId = selectedModelConfigurationId;
        m_selectedRequestorId = selectedRequestorId;

        AZ_Printf("AiAssistantEditorSystemComponent", "Using model : %s", selectedModelConfigurationId.ToString().c_str());
        AZ_Printf("AiAssistantEditorSystemComponent", "Using Requestor : %s", selectedRequestorId.ToString().c_str());
    }

    AZ::Uuid GenAIAsyncRequestSystemComponent::SendPromptToLLM(const AZStd::string& prompt)
    {
        AZ::EntityId selectedModelConfigurationId = m_selectedModelConfigurationId;
        AZ::EntityId selectedRequestorId = m_selectedRequestorId;
        if (!selectedModelConfigurationId.IsValid())
        {
            AZStd::vector<AZ::Component*> activeModelConfigurations;
            GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
                activeModelConfigurations, &GenAIFramework::GenAIFrameworkRequests::GetActiveModelConfigurations);
            if (!activeModelConfigurations.empty())
            {
                selectedModelConfigurationId = activeModelConfigurations.front()->GetEntityId();
                AZ_Warning(
                    "AiAssistantEditorSystemComponent",
                    false,
                    "No model configuration selected. Using the first available model configuration: %s",
                    selectedModelConfigurationId.ToString().c_str())
            }
        }

        if (!selectedRequestorId.IsValid())
        {
            AZStd::vector<AZ::Component*> activeServiceRequesters;
            GenAIFramework::GenAIFrameworkRequestBus::BroadcastResult(
                activeServiceRequesters, &GenAIFramework::GenAIFrameworkRequests::GetActiveServiceRequesters);
            if (!activeServiceRequesters.empty())
            {
                selectedRequestorId = activeServiceRequesters.front()->GetEntityId();
                AZ_Warning(
                    "AiAssistantEditorSystemComponent",
                    false,
                    "No model configuration selected. Using the first available requestor: %s",
                    selectedRequestorId.ToString().c_str())
            }
        }

        AZ::Uuid promptId = AZ::Uuid::CreateRandom();
        GenAIFramework::ModelAPIRequest preparedRequest;
        GenAIFramework::AIModelRequestBus::EventResult(
            preparedRequest, selectedModelConfigurationId, &GenAIFramework::AIModelRequestBus::Events::PrepareRequest, prompt);

        auto callback = [this, promptId, selectedModelConfigurationId](GenAIFramework::ModelAPIResponse outcome)
        {
            AZ::Outcome<AZStd::string, AZStd::string> extractedResponse;
            GenAIFramework::AIModelRequestBus::EventResult(
                extractedResponse, selectedModelConfigurationId, &GenAIFramework::AIModelRequestBus::Events::ExtractResult, outcome);
            if (extractedResponse.IsSuccess())
            {
                AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
                m_promptResponses[promptId] = extractedResponse.GetValue();
            }
            else
            {
                AZStd::unique_lock<AZStd::mutex> lock(m_promptMutex);
                AZStd::string result = "Error: " + extractedResponse.GetError();
                AZ_Warning("AiAssistantEditorSystemComponent", false, "Cannot get a response from the model: %s", result.c_str());
                m_promptResponses[promptId] = result;
            }
        };

        GenAIFramework::AIServiceRequesterBus::Event(
            selectedRequestorId, &GenAIFramework::AIServiceRequesterBus::Events::SendRequest, preparedRequest, callback);

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

} // namespace GenAIFramework
