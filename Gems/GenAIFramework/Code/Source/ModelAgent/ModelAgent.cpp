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
    ModelAgent::ModelAgent(const AZ::EntityId& serviceProviderId, const AZ::EntityId& modelConfigurationId)
        : m_serviceProviderId(serviceProviderId)
        , m_modelConfigurationId(modelConfigurationId)
    {
    }

    void ModelAgent::SendPrompt(
        const AZStd::vector<AZStd::any>& prompt, AZStd::function<void(AZ::Outcome<AZStd::vector<AZStd::any>, AZStd::string>)> callback)
    {
        ModelAPIRequest preparedRequest;
        AIModelRequestBus::EventResult(preparedRequest, m_modelConfigurationId, &AIModelRequestBus::Events::PrepareRequest, prompt);

        auto callbackWrapper = [this, callback, prompt](ModelAPIResponse outcome)
        {
            if (!outcome.IsSuccess())
            {
                callback(AZ::Failure(outcome.GetError()));
                return;
            }
            ModelAPIExtractedResponse extractedResponse;
            AIModelRequestBus::EventResult(
                extractedResponse, m_modelConfigurationId, &AIModelRequestBus::Events::ExtractResult, outcome.GetValue());
            // Add prompt and response to the history if the response is successful
            if (extractedResponse.IsSuccess())
            {
                for (const auto& item : prompt)
                {
                    m_history.push_back({ HistoryTag::Prompt, item });
                }
                m_history.push_back({ HistoryTag::Response, extractedResponse.GetValue() });
            }

            callback(extractedResponse);
        };

        AIServiceProviderBus::Event(m_serviceProviderId, &AIServiceProviderBus::Events::SendRequest, preparedRequest, callbackWrapper);
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
