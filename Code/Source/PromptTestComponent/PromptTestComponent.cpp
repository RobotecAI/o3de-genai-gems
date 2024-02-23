
#include "PromptTestComponent.h"
#include "PromptTestComponent/PromptTestBus.h"
#include <AICore/AICoreActionBus.h>
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
        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<PromptTestBus>("PromptTestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Automation)
                ->Attribute(AZ::Script::Attributes::Category, "AICore")
                ->Attribute(AZ::Script::Attributes::Module, "aicore")
                ->Event("PromptExecute", &PromptTestBus::Events::PromptExecute)
                ->Event("PromptNoExecute", &PromptTestBus::Events::PromptNoExecute)
                ->Event("Execute", &PromptTestBus::Events::Execute)
                ->Event("SetSaveContext", &PromptTestBus::Events::SetSaveContext)
                ->Event("ResetContext", &PromptTestBus::Events::ResetContext)
                ->Event("TestBus", &PromptTestBus::Events::TestBus);
        }
    }

    void PromptTestComponent::TestBus()
    {
        std::cout << "test" << std::endl;
    }

    void PromptTestComponent::PromptExecute(AZStd::string prompt)
    {
        m_promptText = prompt;
        TestPrompt();
    }

    void PromptTestComponent::SetSaveContext(bool saveContext)
    {
        m_saveContext = saveContext;
    }

    void PromptTestComponent::ResetContext()
    {
        m_context.clear();
    }

    void PromptTestComponent::PromptNoExecute(AZStd::string prompt)
    {
        m_promptText = prompt;
        TestPrompt(false);
    }

    void PromptTestComponent::Execute(AZStd::string pythonScript)
    {
        AZStd::string response;
        AICoreActionInterface::Get()->ScriptCall(pythonScript, response, m_aiContext);
    }

    AZ::Crc32 PromptTestComponent::TestPrompt(bool execute)
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
            [this, execute](Aws::Utils::Json::JsonValue response, Aws::Http::HttpResponseCode resultCode)
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
                        if (execute)
                        {
                            AZ_Info("AICore", "Calling script");

                            AZStd::string response;
                            AZStd::string script = prompt.GetValue().first;

                            bool wasCallSuccessful = AICoreActionInterface::Get()->ScriptCall(script, response, m_aiContext);
                            if (wasCallSuccessful)
                            {
                                AZ_Info("AICore", "Script call successful");
                            }
                            else
                            {
                                AZ_Warning("AICore", false, "Script call unsuccessful");
                            }
                        }
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
        PromptTestBus::Handler::BusConnect(GetEntityId());
        if (!m_init)
        {
            m_aiContext.m_key = "Test";
            auto* aicoreinterface = AICoreActionInterface::Get();
            if (!aicoreinterface)
            {
                AZ_Warning("PromptTestComponent", false, "No AICore Interface!");
            }
            aicoreinterface->RegisterAIContext(m_aiContext);
            m_init = true;
        }
    }

    void PromptTestComponent::Deactivate()
    {
        PromptTestBus::Handler::BusDisconnect();
    }

} // namespace AICore