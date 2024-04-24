
#include "ModelAgent.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Communication/AIServiceProviderBus.h>

namespace GenAIFramework
{
    ModelAgent::ModelAgent(AZ::EntityId serviceProviderId, AZ::EntityId modelConfigurationId)
        : m_serviceProviderId(serviceProviderId)
        , m_modelConfigurationId(modelConfigurationId)
    {
    }

    void ModelAgent::SendPrompt(
        const AZStd::vector<AZStd::any>& prompt, AZStd::function<void(AZ::Outcome<AZStd::vector<AZStd::any>, AZStd::string>)> callback)
    {
        ModelAPIRequest preparedRequest;
        AIModelRequestBus::EventResult(preparedRequest, m_modelConfigurationId, &AIModelRequestBus::Events::PrepareRequest, prompt);

        auto callbackWrapper = [this, callback, prompt](ModelAPIProviderResponse outcome)
        {
            if (!outcome.IsSuccess())
            {
                callback(AZ::Failure(outcome.GetError()));
                return;
            }
            ModelAPIResponse extractedResponse;
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

    void ModelAgent::SetServiceProviderId(AZ::EntityId serviceProviderId)
    {
        m_serviceProviderId = serviceProviderId;
    }

    void ModelAgent::SetModelConfigurationId(AZ::EntityId modelConfigurationId)
    {
        m_modelConfigurationId = modelConfigurationId;
    }

} // namespace GenAIFramework
