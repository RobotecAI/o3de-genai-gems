
#include "OllamaContextRequestGeneratorController.h"
#include "OllamaBasicPromptConfiguration.h"
#include <AzCore/Outcome/Outcome.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    OllamaContextRequestGeneratorController::OllamaContextRequestGeneratorController(const OllamaBasicPromptConfiguration& config)
        : m_configuration(config)
    {
    }

    void OllamaContextRequestGeneratorController::Reflect(AZ::ReflectContext* context)
    {
        OllamaBasicPromptConfiguration::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaContextRequestGeneratorController>()->Version(0)->Field(
                "Configuration", &OllamaContextRequestGeneratorController::m_configuration);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaContextRequestGeneratorController>("Ollama Context Request Generator Controller", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaContextRequestGeneratorController::m_configuration,
                        "Configuration",
                        "Configuration for the Ollama context request generator")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
        }
    }

    void OllamaContextRequestGeneratorController::Activate(AZ::EntityId entityId)
    {
        RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::Handler::BusConnect(
            entityId);
    }

    void OllamaContextRequestGeneratorController::Deactivate()
    {
        RequestGeneratorBus<Aws::Utils::Json::JsonValue, AZStd::pair<AZStd::string, AZStd::vector<long long>>>::Handler::BusDisconnect();
    }

    void OllamaContextRequestGeneratorController::SetConfiguration(const OllamaBasicPromptConfiguration& config)
    {
        m_configuration = config;
    }

    const OllamaBasicPromptConfiguration& OllamaContextRequestGeneratorController::GetConfiguration() const
    {
        return m_configuration;
    }

    Aws::Utils::Json::JsonValue OllamaContextRequestGeneratorController::PrepareRequest(
        AZStd::pair<AZStd::string, AZStd::vector<long long>> prompt)
    {
        Aws::Utils::Json::JsonValue jsonValue;

        jsonValue.WithString("prompt", prompt.first.c_str());
        jsonValue.WithString("model", m_configuration.m_model.c_str());

        if (m_configuration.m_format != "")
        {
            jsonValue = jsonValue.WithString("format", m_configuration.m_format.c_str());
        }
        if (m_configuration.m_options != "")
        {
            jsonValue = jsonValue.WithString("options", m_configuration.m_options.c_str());
        }
        if (m_configuration.m_template != "")
        {
            jsonValue = jsonValue.WithString("template", m_configuration.m_template.c_str());
        }

        if (m_configuration.m_keepAlive != "")
        {
            jsonValue = jsonValue.WithString("keep_alive", m_configuration.m_keepAlive.c_str());
        }

        if (m_configuration.m_system != "")
        {
            jsonValue = jsonValue.WithString("system", m_configuration.m_system.c_str());
        }

        jsonValue.WithBool("stream", m_configuration.m_stream);
        // Disabled as sending the raw produces an error
        // jsonValue.WithBool("raw", m_configuration.m_raw);

        Aws::Utils::Array<Aws::Utils::Json::JsonValue> contextArray(prompt.second.size());

        for (int i = 0; i < prompt.second.size(); i++)
        {
            contextArray[i].AsInteger(prompt.second[i]);
        }

        if (contextArray.GetLength() > 0)
        {
            jsonValue.WithArray("context", contextArray);
        }

        return jsonValue;
    }

    AZ::Outcome<AZStd::pair<AZStd::string, AZStd::vector<long long>>, AZStd::string> OllamaContextRequestGeneratorController::
        ProcessRequest(Aws::Utils::Json::JsonValue request)
    {
        AZStd::pair<AZStd::string, AZStd::vector<long long>> response;

        Aws::Utils::Json::JsonView jsonView = request.View();

        if (jsonView.ValueExists("response"))
        {
            response.first = jsonView.GetString("response").c_str();
        }
        else
        {
            return AZ::Failure("No response found in the request");
        }

        if (jsonView.ValueExists("context"))
        {
            auto contextArray = jsonView.GetArray("context");

            for (int i = 0; i < contextArray.GetLength(); i++)
            {
                response.second.push_back(contextArray[i].AsInteger());
            }
        }
        else
        {
            return AZ::Failure("No context found in the request");
        }

        return AZ::Success(response);
    }
} // namespace AICore