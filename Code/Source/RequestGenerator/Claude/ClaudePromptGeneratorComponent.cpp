
#include "ClaudePromptGeneratorComponent.h"
#include "AICore/SystemRegistrationContext/AICoreSystemRegistrationContext.h"
#include "AzCore/RTTI/RTTIMacros.h"
#include "AzCore/Serialization/EditContext.h"
#include "AzCore/Serialization/SerializeContext.h"
#include "RequestGenerator/Claude/ClaudePromptConfiguration.h"
#include <AICore/RequestGenerator/RequestGeneratorBus.h>
#include <AzCore/Component/Component.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    void ClaudePromptGeneratorComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudePromptGeneratorComponent, AZ::Component>()->Version(1)->Field(
                "Default Configuration", &ClaudePromptGeneratorComponent::m_defaultConfiguration);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudePromptGeneratorComponent>("Claude Prompt Generator", "Generates prompts for the Claude model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptGeneratorComponent::m_defaultConfiguration,
                        "Default Configuration",
                        "The default configuration to use when generating prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<AICoreSystemRegistrationContext*>(context))
        {
            registrationContext->RegisterAICoreGenerator<ClaudePromptGeneratorComponent>();
        }
    }

    void ClaudePromptGeneratorComponent::Activate()
    {
        ClaudePromptGeneratorComponentBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudePromptGeneratorComponent::Deactivate()
    {
        ClaudePromptGeneratorComponentBus::Handler::BusDisconnect();
    }

    ClaudePromptInput ClaudePromptGeneratorComponent::GetDefaultPromptInput()
    {
        ClaudePromptInput input;
        input.m_configuration = m_defaultConfiguration;
        return input;
    }

    Aws::Utils::Json::JsonValue ClaudePromptGeneratorComponent::PrepareRequest(ClaudePromptInput prompt)
    {
        Aws::Utils::Json::JsonValue jsonPrompt;
        jsonPrompt.WithString("prompt", prompt.m_prompt.c_str());
        ClaudePromptInputConfiguration configuration = prompt.m_configuration.value_or(m_defaultConfiguration);
        if (configuration.m_maxTokensToSample.has_value())
        {
            jsonPrompt.WithInteger("max_tokens", configuration.m_maxTokensToSample.value());
        }
        if (configuration.m_temperature.has_value())
        {
            jsonPrompt.WithDouble("temperature", configuration.m_temperature.value());
        }
        if (configuration.m_topP.has_value())
        {
            jsonPrompt.WithDouble("top_p", configuration.m_topP.value());
        }
        if (configuration.m_topK.has_value())
        {
            jsonPrompt.WithInteger("top_k", configuration.m_topK.value());
        }
        if (configuration.m_stopSequence.has_value())
        {
            jsonPrompt.WithString("stop_sequence", configuration.m_stopSequence.value().c_str());
        }
        return jsonPrompt;
    }

    AZ::Outcome<ClaudePromptOutput, AZStd::string> ClaudePromptGeneratorComponent::ProcessRequest(Aws::Utils::Json::JsonValue request)
    {
        Aws::Utils::Json::JsonView requestView = request.View();
        ClaudePromptOutput output;

        if (requestView.ValueExists("completion"))
        {
            output.m_completion = requestView.GetString("completion").c_str();
            if (requestView.ValueExists("stop_reason"))
            {
                output.m_stopReason = requestView.GetString("stop_reason").c_str();
            }
            if (requestView.ValueExists("stop"))
            {
                output.stop = requestView.GetString("stop").c_str();
            }
            return AZ::Success(output);
        }
        else
        {
            return AZ::Failure("No completion found in response");
        }
    }

} // namespace AICore