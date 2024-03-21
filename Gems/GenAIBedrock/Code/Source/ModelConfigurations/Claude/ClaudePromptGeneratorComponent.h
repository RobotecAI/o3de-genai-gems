
#pragma once

#include "ModelConfigurations/Claude/ClaudePromptConfiguration.h"
#include <AzCore/Component/Component.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace GenAIBedrock
{
    class ClaudePromptGeneratorComponent
        : public AZ::Component
        , public GenAIFramework::AIModelRequestBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudePromptGeneratorComponent, "{0310426d-3035-4240-b9e5-d1556af98b47}");

        static void Reflect(AZ::ReflectContext* context);

        ClaudePromptGeneratorComponent() = default;
        explicit ClaudePromptGeneratorComponent(const ClaudePromptInputConfiguration& config);
        ~ClaudePromptGeneratorComponent() = default;

        void Activate() override;
        void Deactivate() override;

        // GenAIFramework::ModelConfigurationBus overrides
        GenAIFramework::ModelAPIRequest PrepareRequest(const AZStd::string& prompt) override;
        AZ::Outcome<AZStd::string, AZStd::string> ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse) override;

    private:
        ClaudePromptInputConfiguration m_defaultConfiguration;
    };
} // namespace GenAIBedrock
