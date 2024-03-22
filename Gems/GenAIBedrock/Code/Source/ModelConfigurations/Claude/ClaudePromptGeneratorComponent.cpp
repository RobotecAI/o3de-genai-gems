
#include "ClaudePromptGeneratorComponent.h"
#include "ModelConfigurations/Claude/ClaudePromptConfiguration.h"
#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <sstream>

namespace GenAIBedrock
{
    ClaudePromptGeneratorComponent::ClaudePromptGeneratorComponent(const ClaudePromptInputConfiguration& config)
        : m_defaultConfiguration(config)
    {
    }

    void ClaudePromptGeneratorComponent::Reflect(AZ::ReflectContext* context)
    {
        ClaudePromptInputConfiguration::Reflect(context);

        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudePromptGeneratorComponent, AZ::Component>()->Version(1)->Field(
                "Default Configuration", &ClaudePromptGeneratorComponent::m_defaultConfiguration);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudePromptGeneratorComponent>("Claude Prompt Generator", "Generates prompts for the Claude model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptGeneratorComponent::m_defaultConfiguration,
                        "Default Configuration",
                        "The default configuration to use when generating prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<ClaudePromptGeneratorComponent>();
        }
    }

    void ClaudePromptGeneratorComponent::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudePromptGeneratorComponent::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest ClaudePromptGeneratorComponent::PrepareRequest(const AZStd::string& prompt)
    {
        std::stringstream oss;
        oss << "Human: \"" << prompt.c_str() << "\" Assistant: ";
        Aws::Utils::Json::JsonValue jsonPrompt;
        jsonPrompt.WithString("prompt", oss.str().c_str());
        ClaudePromptInputConfiguration configuration = m_defaultConfiguration;

        jsonPrompt.WithInteger("max_tokens_to_sample", configuration.m_maxTokensToSample);

        if (!configuration.m_useDefaultTemperature)
        {
            jsonPrompt.WithDouble("temperature", configuration.m_temperature);
        }
        if (!configuration.m_useDefaultTopP)
        {
            jsonPrompt.WithDouble("top_p", configuration.m_topP);
        }
        if (!configuration.m_useDefaultTopK)
        {
            jsonPrompt.WithInteger("top_k", configuration.m_topK);
        }
        if (!configuration.m_useDefaultStopSequence)
        {
            jsonPrompt.WithString("stop_sequence", configuration.m_stopSequence.c_str());
        }
        Aws::String jsonString = jsonPrompt.View().WriteReadable();
        AZ_Printf("ClaudePromptGeneratorComponent", "Prepared request: %s", jsonString.c_str());
        return jsonString.c_str();
    }

    AZ::Outcome<AZStd::string, AZStd::string> ClaudePromptGeneratorComponent::ExtractResult(
        const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        if (modelAPIResponse.IsSuccess())
        {
            AZ_Printf("ClaudePromptGeneratorComponent", "Extracted result: %s", modelAPIResponse.GetValue().c_str());
            Aws::Utils::Json::JsonValue jsonPrompt(modelAPIResponse.GetValue().c_str());
            if (jsonPrompt.WasParseSuccessful())
            {
                auto view = jsonPrompt.View();
                return AZ::Success(view.GetString("completion").c_str());
            }
            else
            {
                return AZ::Failure(AZStd::string::format("Failed to parse the response %s", modelAPIResponse.GetValue().c_str()));
            }
        }
        else
        {
            return AZ::Failure(modelAPIResponse.GetError());
        }
    };
} // namespace GenAIBedrock
