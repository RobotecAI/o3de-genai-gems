
#include "PromptTestComponent.h"
#include <AICore/Communication/RequesterBus.h>
#include <AICore/RequestGenerator/RequestGeneratorBus.h>
#include <AzCore/Debug/Trace.h>
#include <AzCore/Math/Crc.h>
#include <AzCore/Serialization/EditContextConstants.inl>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzToolsFramework/ToolsComponents/GenericComponentWrapper.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    void PromptTestComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<PromptTestComponent, AzToolsFramework::Components::EditorComponentBase>()
                ->Version(0)
                ->Field("Save context", &PromptTestComponent::m_saveContext)
                ->Field("Context", &PromptTestComponent::m_context)
                ->Field("Prompt", &PromptTestComponent::m_promptText);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<PromptTestComponent>("Prompt Test", "A component to test prompts")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &PromptTestComponent::m_promptText,
                        "Prompt Text",
                        "The text to prompt the model with")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &PromptTestComponent::m_context, "Context", "The context to use for the prompt")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &PromptTestComponent::m_saveContext,
                        "Should save context between sends",
                        "If true the context will be saved between sends")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->UIElement(AZ::Edit::UIHandlers::Button, "Send prompt")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Send")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &PromptTestComponent::TestPrompt)
                    ->UIElement(AZ::Edit::UIHandlers::Button, "Update reflected context")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Update")
                    ->Attribute(
                        AZ::Edit::Attributes::ChangeNotify,
                        []()
                        {
                            return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
                        });
            }
        }
    }

    AZ::Crc32 PromptTestComponent::TestPrompt()
    {
        AZ_Info("AICore", "Sending prompt: %s", m_promptText.c_str());
        Aws::Utils::Json::JsonValue json_prompt;
        AZStd::pair<AZStd::string, AZStd::vector<long long>> prompt(m_promptText, m_context);
        RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::EventResult(
            json_prompt,
            GetEntityId(),
            &RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::Events::PrepareRequest,
            prompt);

        RequesterBus<Aws::Utils::Json::JsonValue>::Event(
            GetEntityId(),
            &RequesterBus<Aws::Utils::Json::JsonValue>::Events::SendRequest,
            json_prompt,
            [this](Aws::Utils::Json::JsonValue response, Aws::Http::HttpResponseCode resultCode)
            {
                if (resultCode == Aws::Http::HttpResponseCode::OK)
                {
                    AZ::Outcome<AZStd::pair<AZStd::string, AZStd::vector<long long>>, AZStd::string> prompt;
                    AZ_Info("AICore", "Prompt received successfully");
                    RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::EventResult(
                        prompt,
                        GetEntityId(),
                        &RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::Events::
                            ProcessRequest,
                        response);

                    if (prompt.IsSuccess())
                    {
                        AZ_Info("AICore", "Prompt decoded successfully");

                        if (m_saveContext)
                        {
                            m_context = prompt.GetValue().second;
                        }

                        AZ_Info("AICore", "%s", prompt.GetValue().first.c_str());
                    }
                    else
                    {
                        AZ_Warning("AICore", false, "Prompt decoded unsuccessfully");
                    }
                }
                else
                {
                    AZ_Info("AICore", "%d", static_cast<int>(resultCode));
                }
            });
        return AZ::Edit::PropertyRefreshLevels::AttributesAndValues;
    }

    void PromptTestComponent::Activate()
    {
    }

    void PromptTestComponent::Deactivate()
    {
    }

} // namespace AICore