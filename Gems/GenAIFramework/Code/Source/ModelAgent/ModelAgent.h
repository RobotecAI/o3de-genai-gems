
#pragma once

#include <AzCore/Component/EntityId.h>
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/std/any.h>
#include <AzCore/std/containers/vector.h>

namespace GenAIFramework
{
    enum HistoryTag
    {
        Prompt,
        Response
    };

    class ModelAgent
    {
    public:
        ModelAgent() = default;
        ModelAgent(AZ::EntityId serviceProviderId, AZ::EntityId modelConfigurationId);
        ~ModelAgent() = default;

        void SendPrompt(
            const AZStd::vector<AZStd::any>& prompt, AZStd::function<void(AZ::Outcome<AZStd::vector<AZStd::any>, AZStd::string>)> callback);

        void SetServiceProviderId(AZ::EntityId serviceProviderId);
        void SetModelConfigurationId(AZ::EntityId modelConfigurationId);

    private:
        AZStd::vector<AZStd::pair<HistoryTag, AZStd::any>> m_history;

        AZ::EntityId m_serviceProviderId;
        AZ::EntityId m_modelConfigurationId;
    };
} // namespace GenAIFramework
