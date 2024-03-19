
#include "BaseTestComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include "AzCore/Serialization/EditContextConstants.inl"
#include "AzCore/std/string/string.h"
#include "Communication/AwsSdk/AwsSdkBedrockRequesterComponent.h"
#include "ModelConfigurations/Claude/ClaudePromptGeneratorComponent.h"
#include <AzCore/std/string/string.h>

#include <iostream>

namespace AICore
{
    void AIComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<AIComponent, AIComponentBase>()
                ->Field("ModelOutput", &AIComponent::m_modelOutput)
                ->Field("ModelInput", &AIComponent::m_modelInput);

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<AIComponent>("AI Component", "Component for AI")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/AI.png")
                    ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Icons/Components/Viewport/AI.png")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->UIElement(AZ::Edit::UIHandlers::Button, "Test")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Test Button")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &AIComponent::TestButton)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AIComponent::m_modelOutput, "Model Output", "Output from the model")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &AIComponent::m_modelInput, "Model Input", "Input to the model");
            }
        }
    }

    void AIComponent::Activate()
    {
        std::cout << "AIComponent activated" << std::endl;
        std::cout << m_selectedGeneratorName.c_str() << std::endl;
        std::cout << m_selectedRequestorName.c_str() << std::endl;

        std::cout << m_selectedRequestorId.ToString().c_str() << std::endl;
        std::cout << m_selectedGeneratorId.ToString().c_str() << std::endl;
    }

    void AIComponent::Deactivate()
    {
    }

    void AIComponent::TestButton()
    {
        std::cout << "Test button pressed" << std::endl;
        std::cout << m_selectedGeneratorName.c_str() << std::endl;
        std::cout << m_selectedGeneratorId.ToString().c_str() << std::endl;
        // ClaudePromptGeneratorComponentBus::Event(m_selectedGeneratorId, &ClaudePromptGeneratorComponentBus::Events::Test);

        ClaudePromptInput input;
        ClaudePromptGeneratorComponentBus::EventResult(
            input, m_selectedGeneratorId, &ClaudePromptGeneratorComponentBus::Events::GetDefaultPromptInput);

        input.m_prompt = AZStd::string::format("\n\nHuman: %s\n\nAssistant:", m_modelInput.c_str());

        Aws::Utils::Json::JsonValue request;
        ClaudePromptGeneratorComponentBus::EventResult(
            request, m_selectedGeneratorId, &ClaudePromptGeneratorComponentBus::Events::PrepareRequest, input);

        AZStd::function<void(AZ::Outcome<AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError>)> responeCallback =
            [this](AZ::Outcome<AZStd::string, Aws::BedrockRuntime::BedrockRuntimeError> outcome)
        {
            if (outcome.IsSuccess())
            {
                std::cout << "Success" << std::endl;
                m_modelOutput = outcome.GetValue();
                std::cout << outcome.GetValue().c_str() << std::endl;
            }
            else
            {
                std::cout << "Failure" << std::endl;
                std::cout << outcome.GetError().GetMessage().c_str() << std::endl;
            }
        };
        {
        }
        AwsSdkBedrockRequesterBus::Event(m_selectedRequestorId, &AwsSdkBedrockRequesterBus::Events::SendRequest, request, responeCallback);
    }

} // namespace AICore
