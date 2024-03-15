
#pragma once

#include "ModelConfigurations/Claude/ClaudePromptConfiguration.h"
#include <AICore/RequestGenerator/RequestGeneratorBus.h>
#include <AzCore/Component/Component.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    using ClaudePromptGeneratorComponentBus = RequestGeneratorBus<Aws::Utils::Json::JsonValue, ClaudePromptInput, ClaudePromptOutput>;

    class ClaudePromptGeneratorComponent
        : public AZ::Component
        , public ClaudePromptGeneratorComponentBus::Handler
    {
    public:
        AZ_COMPONENT(ClaudePromptGeneratorComponent, "{0310426d-3035-4240-b9e5-d1556af98b47}");

        static void Reflect(AZ::ReflectContext* context);

        ClaudePromptGeneratorComponent() = default;
        explicit ClaudePromptGeneratorComponent(const ClaudePromptInputConfiguration& config);
        ~ClaudePromptGeneratorComponent() = default;

        void Activate() override;
        void Deactivate() override;

        ClaudePromptInput GetDefaultPromptInput() override;
        Aws::Utils::Json::JsonValue PrepareRequest(ClaudePromptInput prompt) override;
        AZ::Outcome<ClaudePromptOutput, AZStd::string> ProcessRequest(Aws::Utils::Json::JsonValue request) override;

    private:
        ClaudePromptInputConfiguration m_defaultConfiguration;
    };
} // namespace AICore
