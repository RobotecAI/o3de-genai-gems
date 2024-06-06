/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ModelAgent.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>

namespace GenAIFramework
{
    ModelAgent::ModelAgent(const AZ::EntityId& serviceProviderId, const AZ::EntityId& modelConfigurationId, AZ::u64 agentId)
        : m_serviceProviderId(serviceProviderId)
        , m_modelConfigurationId(modelConfigurationId)
        , m_agentId(agentId)
    {
        AIModelAgentBus::Handler::BusConnect(m_agentId);
    }

    ModelAgent::~ModelAgent()
    {
        AIModelAgentBus::Handler::BusDisconnect(m_agentId);
    }

    void ModelAgent::SendPrompt(const AZStd::vector<AZStd::any>& prompt)
    {
        ModelAPIRequest preparedRequest;
        AIModelRequestBus::EventResult(preparedRequest, m_modelConfigurationId, &AIModelRequestBus::Events::PrepareRequest, prompt);
        auto callbackWrapper = [this, prompt](ModelAPIResponse outcome)
        {
            if (!outcome.IsSuccess())
            {
                AIModelAgentNotificationBus::Event(
                    m_agentId, &AIModelAgentNotifications::OnPromptResponse, AZ::Failure(outcome.GetError()));
                return;
            }
            ModelAPIExtractedResponse extractedResponse;
            AIModelRequestBus::EventResult(
                extractedResponse, m_modelConfigurationId, &AIModelRequestBus::Events::ExtractResult, outcome.GetValue());
            // Add prompt and response to the history if the response is successful
            if (extractedResponse.IsSuccess())
            {
                m_history.push_back({ prompt, extractedResponse.GetValue() });
            }

            AIModelAgentNotificationBus::Event(m_agentId, &AIModelAgentNotifications::OnPromptResponse, extractedResponse);
        };

        AIServiceProviderBus::Event(m_serviceProviderId, &AIServiceProviderBus::Events::SendRequest, preparedRequest, callbackWrapper);
    }

    AIHistory ModelAgent::GetHistory() const
    {
        return m_history;
    }

    void ModelAgent::SetServiceProviderId(const AZ::EntityId& serviceProviderId)
    {
        m_serviceProviderId = serviceProviderId;
    }

    void ModelAgent::SetModelConfigurationId(const AZ::EntityId& modelConfigurationId)
    {
        m_modelConfigurationId = modelConfigurationId;
    }

} // namespace GenAIFramework
