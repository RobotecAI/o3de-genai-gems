/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "AIAgent.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>

#include <iostream>

namespace GenAIFramework
{
    AIAgent::AIAgent(const AZ::EntityId& serviceProviderId, const AZ::EntityId& modelConfigurationId, AZ::u64 agentId)
        : m_serviceProviderId(serviceProviderId)
        , m_modelConfigurationId(modelConfigurationId)
        , m_agentId(agentId)
    {
        AIAgentRequestBus::Handler::BusConnect(m_agentId);
    }

    AIAgent::~AIAgent()
    {
        AIAgentRequestBus::Handler::BusDisconnect(m_agentId);
    }

    void AIAgent::SendPrompt(const AIMessages& prompt)
    {
        std::cout << "AIAgent::SendPrompt" << std::endl;

        ModelAPIRequest preparedRequest;
        AIModelRequestBus::EventResult(preparedRequest, m_modelConfigurationId, &AIModelRequestBus::Events::PrepareRequest, prompt);
        auto callbackWrapper = [this, prompt](ModelAPIResponse outcome)
        {
            if (!outcome.IsSuccess())
            {
                AIAgentNotificationBus::Event(m_agentId, &AIAgentNotifications::OnAIResponse, AZ::Failure(outcome.GetError()));
                return;
            }
            ModelAPIExtractedResponse extractedResponse;
            AIModelRequestBus::EventResult(
                extractedResponse, m_modelConfigurationId, &AIModelRequestBus::Events::ExtractResult, outcome.GetValue());
            // Add prompt and response to the history if the response is successful
            if (extractedResponse.IsSuccess())
            {
                for (const auto& promptItem : prompt)
                {
                    if (promptItem.first == Role::System)
                    {
                        m_systemMessage = promptItem;
                    }
                    else
                    {
                        m_history.push_back(promptItem);
                    }
                }
                m_history.push_back(extractedResponse.GetValue());
            }

            AIAgentNotificationBus::Event(m_agentId, &AIAgentNotifications::OnAIResponse, extractedResponse);
        };

        AIServiceProviderBus::Event(m_serviceProviderId, &AIServiceProviderBus::Events::SendRequest, preparedRequest, callbackWrapper);
    }

    AIHistory AIAgent::GetHistory() const
    {
        if (m_systemMessage.second.empty())
        {
            return m_history;
        }

        AIHistory historyWithSystemMessage = m_history;
        historyWithSystemMessage.insert(historyWithSystemMessage.begin(), m_systemMessage);

        return historyWithSystemMessage;
    }

    void AIAgent::SetServiceProviderId(const AZ::EntityId& serviceProviderId)
    {
        m_serviceProviderId = serviceProviderId;
    }

    void AIAgent::SetModelConfigurationId(const AZ::EntityId& modelConfigurationId)
    {
        m_modelConfigurationId = modelConfigurationId;
    }

} // namespace GenAIFramework
