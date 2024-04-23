
#include "ModelAgent.h"
#include "GenAIFramework/Communication/AIModelRequestBus.h"
#include "GenAIFramework/Communication/AIServiceProviderBus.h"

#include <iostream>

namespace GenAIFramework
{
    ModelAgent::ModelAgent(AZ::EntityId serviceProviderId, AZ::EntityId modelConfigurationId)
        : m_serviceProviderId(serviceProviderId)
        , m_modelConfigurationId(modelConfigurationId)
    {
    }

    void ModelAgent::SendPrompt(
        const AZStd::vector<AZStd::any>& prompt, AZStd::function<void(AZ::Outcome<AZStd::string, AZStd::string>)> callback)
    {
        std::cout << "ModelAgent::SendPrompt" << std::endl;
        std::cout << m_modelConfigurationId.ToString().c_str() << std::endl;
        std::cout << m_serviceProviderId.ToString().c_str() << std::endl;

        // Add the prompt to the history
        for (const auto& item : prompt)
        {
            m_history.push_back({ HistoryTag::Prompt, item });
        }

        // TODO update vendor gems to use AZStd::any instead of string
        AZStd::string promptString = "";
        for (const auto& item : prompt)
        {
            promptString += AZStd::any_cast<AZStd::string>(item);
        }

        ModelAPIRequest preparedRequest;
        AIModelRequestBus::EventResult(preparedRequest, m_modelConfigurationId, &AIModelRequestBus::Events::PrepareRequest, promptString);

        auto callbackWrapper = [this, callback, prompt](ModelAPIResponse outcome)
        {
            // TODO change AZStd::string to AZStd::vector<AZStd::any>
            AZ::Outcome<AZStd::string, AZStd::string> extractedResponse;
            AIModelRequestBus::EventResult(extractedResponse, m_modelConfigurationId, &AIModelRequestBus::Events::ExtractResult, outcome);
            // Add prompt and response to the history if the response is successful
            if (extractedResponse.IsSuccess())
            {
                for (const auto& item : prompt)
                {
                    m_history.push_back({ HistoryTag::Prompt, item });
                }
                // TODO change to AZStd::any.
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
