
#include "OllamaBasicPromptConfiguration.h"
#include "AzCore/Serialization/EditContextConstants.inl"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    void OllamaBasicPromptConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<OllamaBasicPromptConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("Model", &OllamaBasicPromptConfiguration::m_model)
                ->Field("Format", &OllamaBasicPromptConfiguration::m_format)
                ->Field("Options", &OllamaBasicPromptConfiguration::m_options)
                ->Field("System", &OllamaBasicPromptConfiguration::m_system)
                ->Field("Template", &OllamaBasicPromptConfiguration::m_template)
                ->Field("Stream", &OllamaBasicPromptConfiguration::m_stream)
                ->Field("Raw", &OllamaBasicPromptConfiguration::m_raw)
                ->Field("KeepAlive", &OllamaBasicPromptConfiguration::m_keepAlive);

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<OllamaBasicPromptConfiguration>("Ollama Basic Prompt Configuration", "Configuration of the prompt")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &OllamaBasicPromptConfiguration::m_model, "Model", "Model to use for the prompt")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaBasicPromptConfiguration::m_format,
                        "Format",
                        "The format to return a response in. Currently the only accepted value is json")
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaBasicPromptConfiguration::m_options,
                        "Options",
                        "Additional model parameters listed in the documentation for the Modelfile such as temperature")
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaBasicPromptConfiguration::m_system,
                        "System",
                        "System message to (overrides what is defined in the Modelfile)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::MultiLineEdit,
                        &OllamaBasicPromptConfiguration::m_template,
                        "Template",
                        "The prompt template to use (overrides what is defined in the Modelfile)")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaBasicPromptConfiguration::m_stream,
                        "Stream (disabled as only false is supported)",
                        "If false the response will be returned as a single response object, rather than a stream of objects. Currently "
                        "the only accepted value is false")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaBasicPromptConfiguration::m_raw,
                        "Raw (disabled)",
                        "If true no formatting will be applied to the prompt. You may choose to use the raw parameter if you are "
                        "specifying a full templated prompt in your request to the API")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &OllamaBasicPromptConfiguration::m_keepAlive,
                        "Keep Alive",
                        "Controls how long the model will stay loaded into memory following the request (default: 5m)");
            }
        }
    }
} // namespace AICore